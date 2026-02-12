class PNH_KitManager
{
    private ref PNH_Persistence m_Persistence;
    
    void PNH_KitManager()
    {
        m_Persistence = new PNH_Persistence();
    }

    string GetConfiguredCommand()
    {
        if (m_Persistence) return m_Persistence.GetCommandTrigger();
        return "!bemvindo";
    }

    // --- FUNÇÃO DE ADMIN (RESET) ---
    void AdminResetPlayer(PlayerBase admin, string targetName)
    {
        if (!admin || !admin.GetIdentity()) return;

        string adminID = admin.GetIdentity().GetPlainId();

        if (!m_Persistence.IsAdmin(adminID))
        {
            EnviarMensagemChat(admin, "ERRO: Voce nao tem permissao de Admin.");
            return;
        }

        if (m_Persistence.ResetPlayerData(targetName))
        {
            EnviarMensagemChat(admin, "SUCESSO: Jogador '" + targetName + "' foi resetado.");
            
            PNH_Logger.Log("Welcome", "ADMIN: " + admin.GetIdentity().GetName() + " resetou o kit de " + targetName);
            
            EnviarDiscordInterno("Admin Action", "Admin **" + admin.GetIdentity().GetName() + "** resetou o kit do jogador **" + targetName + "**.", "15158332");
        }
        else
        {
            EnviarMensagemChat(admin, "ERRO: Jogador '" + targetName + "' nao encontrado.");
        }
    }

    // --- FUNÇÃO DE ENTREGA DE KIT ---
    void SpawnWelcomeKit(PlayerBase player, string classRequest)
    {
        if (!player || !player.GetIdentity()) return;

        string steamID = player.GetIdentity().GetPlainId();
        string playerName = player.GetIdentity().GetName();

        if (m_Persistence.HasPlayerReceivedKit(steamID))
        {
            EnviarMensagemChat(player, "ERRO: Voce ja resgatou seu kit de boas-vindas!");
            return;
        }

        PNH_ClassDefinition selectedClass = null;
        if (classRequest != "") selectedClass = m_Persistence.GetClassByName(classRequest);
        
        // Se não achou ou não pediu, tenta pegar a primeira
        if (!selectedClass)
        {
            array<ref PNH_ClassDefinition> allClasses = m_Persistence.GetClassesConfig();
            if (allClasses && allClasses.Count() > 0)
            {
                 selectedClass = allClasses.Get(0); 
            }
        }

        if (!selectedClass)
        {
            EnviarMensagemChat(player, "ERRO CRITICO: Nenhuma classe configurada no servidor.");
            return;
        }

        // CRIAÇÃO DO BAU
        vector pos = player.GetPosition();
        EntityAI chest = EntityAI.Cast(GetGame().CreateObject("SeaChest", pos));
        
        if (!chest) return;

        foreach (PNH_KitItem itemDef : selectedClass.Items)
        {
            for (int i = 0; i < itemDef.quantity; i++)
            {
                EntityAI item = chest.GetInventory().CreateInInventory(itemDef.type);
                if (item)
                {
                     foreach (string att : itemDef.attachments)
                     {
                         item.GetInventory().CreateInInventory(att);
                     }
                }
            }
        }

        m_Persistence.RegisterPlayer(steamID, playerName, selectedClass.ClassName);
        
        EnviarMensagemChat(player, "SUCESSO: Seu kit esta dentro do Bau a sua frente!");
        
        PNH_Logger.Log("Welcome", "SUCESSO: " + playerName + " (" + steamID + ") pegou o kit " + selectedClass.ClassName);
        
        string mensagemDiscord = "O jogador **" + playerName + "** |  ID: `" + steamID + "`  |  Resgatou o Kit: **" + selectedClass.ClassName + "** (SeaChest)";
        EnviarDiscordInterno("Entrega Confirmada", mensagemDiscord, "3066993");
    }

    void EnviarMensagemChat(PlayerBase player, string mensagem)
    {
        if (player && player.GetIdentity())
        {
            Param1<string> m = new Param1<string>("[PNH] " + mensagem);
            GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m, true, player.GetIdentity());
        }
    }

    // --- INTEGRACAO COM PNH_CORE (Atualizada 2.1) ---
    void EnviarDiscordInterno(string titulo, string msg, string cor)
    {
        // Agora pega a URL do Core (Categoria Welcome)
        string webhookURL = PNH_CoreConfig.GetWelcomeURL();
        
        // Backup: Se não tiver URL de Welcome, usa a URL de Sistema
        if (webhookURL == "") webhookURL = PNH_CoreConfig.GetSystemURL();

        int corInt = 16777215; // Branco
        if (cor == "3066993") corInt = 3066993;     // Verde
        if (cor == "15158332") corInt = 15158332;   // Vermelho

        // Envia via Core
        PNH_Discord.Send(titulo, msg, corInt, webhookURL);
    }
}
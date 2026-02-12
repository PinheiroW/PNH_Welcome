modded class MissionServer
{
    ref PNH_KitManager m_PNHKitManager;

    override void OnInit()
    {
        super.OnInit();
        m_PNHKitManager = new PNH_KitManager();
        
        // Log usando o PNH_Core
        PNH_Logger.Log("Welcome", "Kit Manager Carregado.");
    }

    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);
        
        if (eventTypeId == ChatMessageEventTypeID)
        {
            ChatMessageEventParams chatParams = ChatMessageEventParams.Cast(params);
            
            if (chatParams)
            {
                string senderName = chatParams.param2;
                string message = chatParams.param3;
                
                if (senderName != "") 
                {
                    message.ToLower(); 
                    
                    string comandoConfigurado = m_PNHKitManager.GetConfiguredCommand();
                    comandoConfigurado.ToLower(); 

                    TStringArray palavras = new TStringArray;
                    message.Split(" ", palavras);

                    if (palavras.Count() > 0)
                    {
                        string comandoDigitado = palavras[0];
                        
                        // 1. COMANDO DE JOGADOR
                        if (comandoDigitado == comandoConfigurado)
                        {
                            string argumentoClasse = "";
                            if (palavras.Count() > 1) argumentoClasse = palavras[1];

                            PlayerBase player = PNH_GetPlayerByName(senderName);
                            if (player) m_PNHKitManager.SpawnWelcomeKit(player, argumentoClasse);
                        }
                        
                        // 2. COMANDO DE ADMIN
                        if (comandoDigitado == "!kitreset")
                        {
                            if (palavras.Count() > 1)
                            {
                                string alvoNome = palavras[1]; 
                                PlayerBase admin = PNH_GetPlayerByName(senderName);
                                if (admin) m_PNHKitManager.AdminResetPlayer(admin, alvoNome);
                            }
                        }
                    }
                }
            }
        }
    }

    PlayerBase PNH_GetPlayerByName(string name)
    {
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);
        foreach (Man man : players)
        {
            if (man && man.GetIdentity())
            {
                if (man.GetIdentity().GetName() == name) return PlayerBase.Cast(man);
            }
        }
        return null;
    }
}
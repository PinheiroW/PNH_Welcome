// ---------------------------------------------------------
// PNH_Persistence.c - SISTEMA DE DADOS + ADMIN
// ---------------------------------------------------------

class PNH_ConfigJSON
{
    // Removemos DiscordWebhookURL daqui
    string CommandTrigger;
    ref array<string> Admins; 

    void PNH_ConfigJSON()
    {
        CommandTrigger = "!bemvindo"; 
        Admins = new array<string>;
        Admins.Insert("76561198000000000"); 
    }
}

class PlayerInfoPNH
{
    string SteamID;
    string Nome;
    string DataHora;
    string ClasseEscolhida;

    void PlayerInfoPNH(string id, string nome, string data, string classe)
    {
        SteamID = id;
        Nome = nome;
        DataHora = data;
        ClasseEscolhida = classe;
    }
}

class PNH_ClassDefinition
{
    string ClassName;
    ref array<ref PNH_KitItem> Items;
    void PNH_ClassDefinition() { Items = new array<ref PNH_KitItem>; }
}

class PNH_KitItem
{
    string type;
    int quantity;
    ref array<string> attachments;
    void PNH_KitItem() { attachments = new array<string>(); }
}

class PNH_Persistence
{
    private static const string PLAYERS_FILE = "$profile:PNH/Settings/players.json";
    private static const string ITEMS_FILE = "$profile:PNH/Settings/classes.json";
    private static const string CONFIG_FILE = "$profile:PNH/Settings/config.json";

    ref array<ref PlayerInfoPNH> m_PlayersData;
    ref array<ref PNH_ClassDefinition> m_ClassesConfig;
    ref PNH_ConfigJSON m_Config;

    void PNH_Persistence()
    {
        m_PlayersData = new array<ref PlayerInfoPNH>;
        m_ClassesConfig = new array<ref PNH_ClassDefinition>;
        m_Config = new PNH_ConfigJSON();

        if (!FileExist("$profile:PNH")) MakeDirectory("$profile:PNH");
        if (!FileExist("$profile:PNH/Settings")) MakeDirectory("$profile:PNH/Settings");

        LoadPlayers();
        LoadClasses();
        LoadConfig();
    }

    array<ref PNH_ClassDefinition> GetClassesConfig() { return m_ClassesConfig; }

    string GetCommandTrigger()
    {
        if (m_Config) return m_Config.CommandTrigger;
        return "!bemvindo";
    }

    bool IsAdmin(string steamID)
    {
        if (!m_Config || !m_Config.Admins) return false;
        foreach (string adminId : m_Config.Admins)
        {
            if (adminId == steamID) return true;
        }
        return false;
    }

    void RegisterPlayer(string id, string nome, string classe)
    {
        PlayerInfoPNH newPlayer = new PlayerInfoPNH(id, nome, GetTimestamp(), classe);
        m_PlayersData.Insert(newPlayer);
        SavePlayers();
    }

    bool HasPlayerReceivedKit(string id)
    {
        foreach (PlayerInfoPNH p : m_PlayersData)
        {
            if (p.SteamID == id) return true;
        }
        return false;
    }

    bool ResetPlayerData(string nomeAlvo)
    {
        nomeAlvo.ToLower();
        for (int i = 0; i < m_PlayersData.Count(); i++)
        {
            string pNome = m_PlayersData.Get(i).Nome;
            pNome.ToLower();
            if (pNome == nomeAlvo)
            {
                m_PlayersData.Remove(i);
                SavePlayers();
                return true;
            }
        }
        return false;
    }

    private void LoadPlayers()
    {
        if (FileExist(PLAYERS_FILE)) JsonFileLoader<array<ref PlayerInfoPNH>>.JsonLoadFile(PLAYERS_FILE, m_PlayersData);
    }

    private void SavePlayers()
    {
        JsonFileLoader<array<ref PlayerInfoPNH>>.JsonSaveFile(PLAYERS_FILE, m_PlayersData);
    }

    private void LoadConfig()
    {
        if (FileExist(CONFIG_FILE)) JsonFileLoader<PNH_ConfigJSON>.JsonLoadFile(CONFIG_FILE, m_Config);
        else 
        {
            JsonFileLoader<PNH_ConfigJSON>.JsonSaveFile(CONFIG_FILE, m_Config);
        }
    }

    private void LoadClasses()
    {
        if (FileExist(ITEMS_FILE)) JsonFileLoader<array<ref PNH_ClassDefinition>>.JsonLoadFile(ITEMS_FILE, m_ClassesConfig);
        else
        {
            PNH_ClassDefinition c1 = new PNH_ClassDefinition(); c1.ClassName = "sobrevivente";
            PNH_KitItem i1 = new PNH_KitItem(); i1.type = "M4A1"; i1.quantity = 1; i1.attachments = {"Mag_STANAG_30Rnd"};
            c1.Items.Insert(i1); m_ClassesConfig.Insert(c1);
            JsonFileLoader<array<ref PNH_ClassDefinition>>.JsonSaveFile(ITEMS_FILE, m_ClassesConfig);
        }
    }

    PNH_ClassDefinition GetClassByName(string name)
    {
        name.ToLower();
        foreach (PNH_ClassDefinition def : m_ClassesConfig) {
            string cName = def.ClassName; cName.ToLower();
            if (cName == name) return def;
        }
        return null;
    }

    string GetAvailableClassesString()
    {
        string lista = "";
        foreach (PNH_ClassDefinition def : m_ClassesConfig) lista += "[" + def.ClassName + "] ";
        return lista;
    }

    string GetTimestamp()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day); GetHourMinuteSecond(hour, minute, second);
        return day.ToString() + "/" + month.ToString() + "/" + year.ToString() + " " + hour.ToString() + ":" + minute.ToString();
    }
}
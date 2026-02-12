class CfgPatches
{
	class PNH_Welcome
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
			"PNH_Core"
		};
	};
};
class CfgMods
{
	class PNH_Welcome
	{
		dir="PNH_Welcome";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="PNH Welcome System";
		credits="Pinheiro";
		author="Pinheiro";
		authorID="0";
		version="1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				value="";
				files[]=
				{
					"PNH_Welcome/Scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"PNH_Welcome/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value="";
				files[]=
				{
					"PNH_Welcome/Scripts/5_Mission"
				};
			};
		};
	};
};

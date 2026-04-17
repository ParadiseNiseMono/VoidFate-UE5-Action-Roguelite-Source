// Paradise NiseMono All Rights Reserved


#include "VFGameplayTags.h"

namespace VFGameplayTags
{
	//Input
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look");
	UE_DEFINE_GAMEPLAY_TAG(Input_Dodge, "Input.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Input_SwitchTarget, "Input.SwitchTarget");
	UE_DEFINE_GAMEPLAY_TAG(Input_SpecialWeaponAbility_Light, "Input.SpecialWeaponAbility.Light");
	UE_DEFINE_GAMEPLAY_TAG(Input_SpecialWeaponAbility_Heavy, "Input.SpecialWeaponAbility.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Input_PickUp, "Input.PickUp");
	UE_DEFINE_GAMEPLAY_TAG(Input_Sprint, "Input.Sprint");
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Equip_Katana, "Input.Equip.Katana");
	
	UE_DEFINE_GAMEPLAY_TAG(Input_Unequip_Katana, "Input.Unequip.Katana");

	UE_DEFINE_GAMEPLAY_TAG(Input_LightAttack_Katana, "Input.LightAttack.Katana");
	UE_DEFINE_GAMEPLAY_TAG(Input_HeavyAttack_Katana, "Input.HeavyAttack.Katana");

	UE_DEFINE_GAMEPLAY_TAG(Input_MustHold, "Input.MustHold");
	UE_DEFINE_GAMEPLAY_TAG(Input_MustHold_Block, "Input.MustHold.Block");

	UE_DEFINE_GAMEPLAY_TAG(Input_Toggleable, "Input.Toggleable");
	UE_DEFINE_GAMEPLAY_TAG(Input_Toggleable_TargetLock, "Input.Toggleable.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(Input_Toggleable_Transcendence, "Input.Toggleable.Transcendence");

	//Player
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Katana, "Player.Ability.Equip.Katana");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Katana, "Player.Ability.Unequip.Katana");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Katana, "Player.Ability.Attack.Light.Katana");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Katana, "Player.Ability.Attack.Heavy.Katana");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitPause, "Player.Ability.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Dodge, "Player.Ability.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Block, "Player.Ability.Block");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLock, "Player.Ability.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Transcendence, "Player.Ability.Transcendence");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SpecialWeaponAbility_Light, "Player.Ability.SpecialWeaponAbility.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SpecialWeaponAbility_Heavy, "Player.Ability.SpecialWeaponAbility.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Execute, "Player.Ability.Execute");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Stunned, "Player.Ability.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Movement_Sprint, "Player.Ability.Movement.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_VoidTime, "Player.Ability.VoidTime");

	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_SpecialWeaponAbility_Light, "Player.Cooldown.SpecialWeaponAbility.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_SpecialWeaponAbility_Heavy, "Player.Cooldown.SpecialWeaponAbility.Heavy");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Katana, "Player.Weapon.Katana");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Katana, "Player.Event.Equip.Katana");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Katana, "Player.Event.Unequip.Katana");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause, "Player.Event.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SuccessfulBlock, "Player.Event.SuccessfulBlock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_PerfectBlock, "Player.Event.PerfectBlock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_PerfectDodge, "Player.Event.PerfectDodge");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Left,"Player.Event.SwitchTarget.Left");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Right,"Player.Event.SwitchTarget.Right");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ActivateTranscendence,"Player.Event.ActivateTranscendence");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_AOE,"Player.Event.AOE");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_PickUpItems,"Player.Event.PickUpItems");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Execute,"Player.Event.Execute");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_KilledEnemy, "Player.Event.KilledEnemy");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_JumpToFinisher, "Player.Status.JumpToFinisher");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Dodging, "Player.Status.Dodging");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Blocking, "Player.Status.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLock, "Player.Status.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Transcendence_Activating, "Player.Status.Transcendence.Activating");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Transcendence_Active, "Player.Status.Transcendence.Active");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Stamina_Full, "Player.Status.Stamina.Full");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Stamina_None, "Player.Status.Stamina.None");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_CanDodgeCounter, "Player.Status.CanDodgeCounter");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Movement_Sprint, "Player.Status.Movement.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_VoidTime, "Player.Status.VoidTime");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_PerfectDodgeWindow, "Player.Status.PerfectDodgeWindow");

	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Heavy, "Player.SetByCaller.AttackType.Heavy");

	UE_DEFINE_GAMEPLAY_TAG(Player_PickUp_Stones, "Player.PickUp.Stones");

	//Enemy
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, "Enemy.Ability.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_DashStrike, "Enemy.Ability.DashStrike");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_BeenPerfectBlocked, "Enemy.Ability.BeenPerfectBlocked");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Rush, "Enemy.Ability.Rush");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SummonEnemies, "Enemy.Ability.SummonEnemies");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SpawnItem, "Enemy.Ability.SpawnItem");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Executed, "Enemy.Ability.Executed");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Stunned, "Enemy.Ability.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_HitPause, "Enemy.Ability.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SpecialAttack, "Enemy.Ability.SpecialAttack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Teleport, "Enemy.Ability.Teleport");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_BeenPerfectBlocked, "Enemy.Event.BeenPerfectBlocked");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SummonEnemies, "Enemy.Event.SummonEnemies");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Executed, "Enemy.Event.Executed");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_HitPause, "Enemy.Event.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_VoidBomb, "Enemy.Event.VoidBomb");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Ultimate, "Enemy.Event.Ultimate");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Rush, "Enemy.Event.Rush");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SpecialAttack, "Enemy.Event.SpecialAttack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Teleport, "Enemy.Event.Teleport");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing, "Enemy.Status.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack, "Enemy.Status.UnderAttack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnBlockable, "Enemy.Status.UnBlockable");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Cooldown_HitReact, "Enemy.Cooldown.HitReact");

	//Shared
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_VoidSpeed, "Shared.Ability.VoidSpeed");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_SpawnProjectile, "Shared.Event.SpawnProjectile");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Jump, "Shared.Event.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_PlayCinematicCamera, "Shared.Event.PlayCinematicCamera");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_AOE, "Shared.Event.AOE");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead, "Shared.Status.Dead");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Stunned, "Shared.Status.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Front, "Shared.Status.HitReact.Front");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Left, "Shared.Status.HitReact.Left");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Right, "Shared.Status.HitReact.Right");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Back, "Shared.Status.HitReact.Back");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Invincible, "Shared.Status.Invincible");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_PostureRecoveryDelay, "Shared.Status.PostureRecoveryDelay");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Posture_Full, "Shared.Status.Posture.Full");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Posture_None, "Shared.Status.Posture.None");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_VoidSpeed, "Shared.Status.VoidSpeed");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_PoiseStack, "Shared.Status.PoiseStack");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_BlockCancelAbility, "Shared.Status.BlockCancelAbility");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Entry_PlayAnimation, "Shared.Status.Entry.PlayAnimation");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Tutorial_Movement, "Shared.Status.Tutorial.Movement");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Tutorial_Equip, "Shared.Status.Tutorial.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Tutorial_Combat, "Shared.Status.Tutorial.Combat");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Tutorial_Execute, "Shared.Status.Tutorial.Execute");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Tutorial_PerfectDodge, "Shared.Status.Tutorial.PerfectDodge");
	
	//Frontend Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenu, "Frontend.WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameHud, "Frontend.WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend");

	//Frontend Widgets
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKeyScreen, "Frontend.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenuScreen, "Frontend.Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_StoryScreen, "Frontend.Widget.StoryScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_OptionsScreen, "Frontend.Widget.OptionsScreen");

	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_ConfirmScreen, "Frontend.Widget.ConfirmScreen");

	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_KeyRemapScreen, "Frontend.Widget.KeyRemapScreen");
	
	//Frontend Options Image
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Image_TestImage, "Frontend.Image.TestImage");

	//GameData
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_SurvivalGameModeMap, "GameData.Level.SurvivalGameModeMap");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_SurvivalGameModeMap_Easy, "GameData.Level.SurvivalGameModeMap.Easy");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_SurvivalGameModeMap_Normal, "GameData.Level.SurvivalGameModeMap.Normal");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_SurvivalGameModeMap_Hard, "GameData.Level.SurvivalGameModeMap.Hard");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_SurvivalGameModeMap_VeryHard, "GameData.Level.SurvivalGameModeMap.VeryHard");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_MainMenuMap, "GameData.Level.MainMenuMap");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_TutorialMap, "GameData.Level.TutorialMap");

	//Reward
	UE_DEFINE_GAMEPLAY_TAG(Reward_Generic_Common_AttackPowerBoost, "Reward.Generic.Common.AttackPowerBoost");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Generic_Common_HealthBoost, "Reward.Generic.Common.HealthBoost");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Generic_Common_StaminaBoost, "Reward.Generic.Common.StaminaBoost");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Generic_Common_DefensePowerBoost, "Reward.Generic.Common.DefensePowerBoost");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Generic_Rare_Swift, "Reward.Generic.Rare.Swift");

	UE_DEFINE_GAMEPLAY_TAG(Reward_Combat, "Reward.Combat");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Combat_Common_TranscendenceEat, "Reward.Combat.Common.TranscendenceEat");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Combat_Common_VoidTimeEat, "Reward.Combat.Common.VoidTimeEat");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Combat_Rare_SprintBomb, "Reward.Combat.Rare.SprintBomb");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Combat_SuperRare_SprintShadowBomb, "Reward.Combat.SuperRare.SprintShadowBomb");

	UE_DEFINE_GAMEPLAY_TAG(Reward_Posture, "Reward.Posture");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Posture_Common_PostureBoost, "Reward.Posture.Common.PostureBoost");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Posture_Common_PostureDamageBoost, "Reward.Posture.Common.PostureDamageBoost");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Posture_Common_PerfectBlockHeal, "Reward.Posture.Common.PerfectBlockHeal");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Posture_Rare_ExecuteHealBomb, "Reward.Posture.Rare.ExecuteHealBomb");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Posture_SuperRare_PerfeckBlockDefenseUp, "Reward.Posture.SuperRare.PerfeckBlockDefenseUp");

	UE_DEFINE_GAMEPLAY_TAG(Reward_Dodge, "Reward.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Dodge_Common_PerfectDodgeHeal, "Reward.Dodge.Common.PerfectDodgeHeal");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Dodge_Common_DodgeCostReduce, "Reward.Dodge.Common.DodgeCostReduce");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Dodge_Rare_PerfectDodgeBomb, "Reward.Dodge.Rare.PerfectDodgeBomb");
	UE_DEFINE_GAMEPLAY_TAG(Reward_Dodge_SuperRare_PerfectDodgePowerUp, "Reward.Dodge.SuperRare.PerfectDodgePowerUp");
}

# VoidFate-UE5-Action-Roguelite-Source
  這是我花了四個月單人開發的專案。為了確保效能與可擴展性，我捨棄了純藍圖開發。 底層戰鬥我深入客製化了 GAS 的 ExecCalc 與 Ability Tasks； 系統架構我全面採用 Subsystem 與 Interface 避免硬引用； UI 層我利用 C++ Data Object 結合 CommonUI 實作虛擬化列表，徹底解決了 UI Tick 效能瓶頸。 這不僅是一款遊戲 Demo，更是一個以 3A 專案標準打造的系統架構雛形。

# ⚔️ Void Fate - Technical Architecture & Deep Dive
《Void Fate》 是一款基於 Unreal Engine 5 開發的 3D 動作生存遊戲。
本專案的核心目標不在於完成一款商業遊戲，而是展現現代化 UE5 C++ 客戶端開發的業界標準。全專案高度採用模組化 (Modular)、資料驅動 (Data-Driven) 以及低耦合 (Decoupling) 的架構設計。

🕒 開發者旅程 (The Journey)
2025/09 - 2025/12 ｜ 系統化底層建構：透過 Udemy 高強度鑽研 UE5 C++ 核心與 Gameplay Ability System (GAS) 3A 級戰鬥架構。

2026/01 - 2026/04 ｜ 極限開發期：在兼顧大學課業與【艾玩天地 (Iwplay World)】客戶端程式實習的極限狀態下，獨立由零到一完成本專案的所有程式碼架構與系統實作。

🛠️ 核心技術棧 (Tech Stack)
Game Framework: C++ (90%) / Blueprints (10%, 僅用於視覺與資源組裝)

Combat System: Gameplay Ability System (GAS)

UI Framework: Common UI / MVVM Pattern / Data Registry

AI System: Behavior Tree / Custom BTTask & BTService

Architecture: Subsystems / Actor Components / Data-Driven Design****

# 1. 核心程式哲學：高度解耦的模組化設計 (Modular Architecture)
為了保證專案的極高可擴展性，並確保在後續除錯時能快速定位問題，本專案嚴格遵守「邏輯分離」與「低耦合」原則。

1.1 基於 Component 的邏輯分離
傳統新手常將所有邏輯塞入 Character 類別中，導致程式碼臃腫且難以重用。在《Void Fate》中，Character 僅作為一個「容器」，真正的邏輯被拆分至專屬的 Component 中：

PawnCombatComponent: 專責處理戰鬥相關的輸入、GAS 技能觸發與武器裝備。

PawnUIComponent: 專責處理與 UI 相關的資料綁定與更新。

優勢：當未來需要新增不同類型的敵人或玩家角色時，只需掛載對應的 Component，即可瞬間獲得戰鬥或 UI 能力，實現邏輯的高度重用。

1.2 GameInstance Subsystems 的全域管理
為了避免 Singleton 的濫用，並確保管理系統的生命週期與遊戲本身一致，本專案大量使用自定義的 Subsystem 來處理跨物件的全域邏輯：

VFUISubsystem: 統籌 Common UI 的路由、Widget 堆疊與設備焦點追蹤。

VFExecutionCameraSubsystem: 專門處理處決運鏡，將「運鏡邏輯」從「戰鬥邏輯」中完美抽離。

VFRewardSubsystem: 管理 Rogue-lite 機制下的動態獎勵派發。

# 2. 前端革命：資料驅動的 UI 與 Common UI 實戰
(開發初期最耗費心力，為解決「難以維護」與「手把熱切換焦點遺失」痛點而生。)

2.1 MVVM 架構與 DataRegistry 實作
為了徹底告別混亂的 UI 藍圖 (Blueprint Spaghetti)，我採用了資料層與表現層分離的架構。所有 UI 所需的選單資料（如設定選項、玩家屬性）皆於 C++ 內部構建並封裝。
<img width="1980" height="1359" alt="螢幕擷取畫面 2026-04-17 202340" src="https://github.com/user-attachments/assets/31ed3494-d530-46ca-a5cd-4b863a93c20b" />

2.2 解決多端適配痛點：設備熱切換 (Hot-Swapping)
導入 Epic 官方 Common UI 框架，實作 ActivatableWidgets 與自定義路由。不僅解決了多層選單的堆疊衝突，更完美支援「鍵盤/滑鼠」與「手把」的無縫切換，確保在設備轉換時，UI 焦點 (Focus) 能夠精準追蹤而不遺失。
<img width="1280" height="720" alt="VoidFate   2026-04-17 19-31-30" src="https://github.com/user-attachments/assets/139d318e-5149-4163-b6b8-142df11eb6f7" />

# ⚔️ 3. 核心戰鬥架構：3A 級 Gameplay Ability System (GAS) 深度實作
《Void Fate》的戰鬥底層完全基於 Unreal Engine 的 Gameplay Ability System (GAS) 打造。本專案不單純只是使用基本的 GameplayAbility，而是深入自定義了 Task、MMC、ExecCalc 以及與 AI 行為樹的橋接，以實現高度的擴展性與極致的戰鬥手感。

3.1 資料驅動的初始化與 Rogue-like 動態擴展 (Data-Driven & Runtime Grants)
為了讓企劃能完全掌控數值與技能分配，專案徹底捨棄了 C++ 寫死的做法：

啟動資料 (Startup Data)：透過實作 DataAsset_StartUpDataBase (及其子類別 Ninja / Enemy)，所有角色在生成的瞬間，其初始屬性、預設 Gameplay Abilities (GA) 與 Gameplay Effects (GE) 都是透過 DataAsset 讀取並賦予。

Rogue-like 動態獎勵系統：結合自定義的 VFRewardSubsystem，玩家在遊戲過程中獲得的強化（如：攻擊附帶吸血、攻速提升等），本質上是在 Runtime 階段動態賦予全新的 GA 與 GE。這證明了本架構具備極高的動態擴展能力，能輕易支撐肉鴿遊戲的隨機性。

3.2 徹底解耦的 UI 屬性廣播 (Delegate Events)
在傳統做法中，UI 經常透過 Tick 去抓取玩家血量，這會造成極大的效能浪費與高耦合。
本專案在 VFAttributeSet 中實作了完整的 Delegate 廣播機制。當 GAS 內部的屬性（如 Health、Posture）發生變化時，會主動發送 Event。前端的 UI 組件僅需綁定這些 Delegate 即可被動更新，UI 系統完全不知道 GAS 的存在，實現了底層數值與前端視覺的完美解耦。
<img width="3837" height="2159" alt="螢幕擷取畫面 2026-04-17 195945" src="https://github.com/user-attachments/assets/a451b071-6d7a-4dfe-a4fe-59b3f0a85649" />

3.3 突破壁壘：GAS 與 AI 行為樹的深度整合 (AI Target Lock-on)
這是本專案最具技術含量的挑戰之一。傳統 AI 行為樹難以精準控制複雜的技能邏輯，為此我開發了專屬的行為樹節點：

BTTask_ActivateAbilityWithTarget：這讓 AI 能夠直接從行為樹根據 Blackboard 的邏輯，精準觸發對應的 Gameplay Tag 來施放 GAS 技能。

動態目標追蹤 (Dynamic Target Tracking)：
為了解決敵方導彈或突進技能「無法死死咬住玩家」的痛點，我在此 Task 中，將玩家即時的 Actor 資訊封裝為 FGameplayAbilityTargetDataHandle，並透過 Event Payload 傳遞給 GA。

實戰效果：不論玩家如何靈活位移，敵方的追擊技能都能從 TargetData 中實時解算玩家座標，實現壓迫感極強的高難度追蹤攻擊。
https://github.com/user-attachments/assets/92e18d52-22bc-4ec7-929a-04ec0e14ba04





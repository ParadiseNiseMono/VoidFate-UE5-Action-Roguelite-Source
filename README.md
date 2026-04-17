# VoidFate-UE5-Action-Roguelite-Source
這是我花了四個月單人開發的專案。為了確保效能與可擴展性，我捨棄了純藍圖開發。 底層戰鬥我深入客製化了 GAS 的 ExecCalc 與 Ability Tasks； 系統架構我全面採用 Subsystem 與 Interface 避免硬引用； UI 層我利用 C++ Data Object 結合 CommonUI 實作虛擬化列表，徹底解決了 UI Tick 效能瓶頸。 這不僅是一款遊戲 Demo，更是一個以 3A 專案標準打造的系統架構雛形。

# ⚔️ Void Fate - Technical Architecture & Deep Dive
《Void Fate》 是一款基於 Unreal Engine 5 開發的 3D 動作生存遊戲。
本專案的核心目標不在於完成一款商業遊戲，而是展現現代化 UE5 C++ 客戶端開發的業界標準。全專案高度採用模組化 (Modular)、資料驅動 (Data-Driven) 以及低耦合 (Decoupling) 的架構設計。

## 🕒 開發者旅程 (The Journey)
2025/09 - 2025/12 ｜ 系統化底層建構：透過 Udemy 高強度鑽研 UE5 C++ 核心與 Gameplay Ability System (GAS) 3A 級戰鬥架構。

2026/01 - 2026/04 ｜ 極限開發期：在兼顧大學課業與【艾玩天地 (Iwplay World)】行銷部實習的極限狀態下，獨立由零到一完成本專案的所有程式碼架構與系統實作。

## 🛠️ 核心技術棧 (Tech Stack)
Game Framework: C++ (90%) / Blueprints (10%, 僅用於視覺與資源組裝)

Combat System: Gameplay Ability System (GAS)

UI Framework: Common UI / MVVM Pattern / Data Registry

AI System: Behavior Tree / Custom BTTask & BTService

Architecture: Subsystems / Actor Components / Data-Driven Design****


# 1. 核心程式哲學：高度解耦的模組化設計 (Modular Architecture)
為了保證專案的極高可擴展性，並確保在後續除錯時能快速定位問題，本專案嚴格遵守「邏輯分離」與「低耦合」原則。
<details>
<summary>點擊展開說明</summary>
  
## 1.1 基於 Component 的邏輯分離
傳統新手常將所有邏輯塞入 Character 類別中，導致程式碼臃腫且難以重用。在《Void Fate》中，Character 僅作為一個「容器」，真正的邏輯被拆分至專屬的 Component 中：

PawnCombatComponent: 專責處理戰鬥相關的輸入、GAS 技能觸發與武器裝備。

PawnUIComponent: 專責處理與 UI 相關的資料綁定與更新。

優勢：當未來需要新增不同類型的敵人或玩家角色時，只需掛載對應的 Component，即可瞬間獲得戰鬥或 UI 能力，實現邏輯的高度重用。

## 1.2 GameInstance Subsystems 的全域管理
為了避免 Singleton 的濫用，並確保管理系統的生命週期與遊戲本身一致，本專案大量使用自定義的 Subsystem 來處理跨物件的全域邏輯：

VFUISubsystem: 統籌 Common UI 的路由、Widget 堆疊與設備焦點追蹤。

VFExecutionCameraSubsystem: 專門處理處決運鏡，將「運鏡邏輯」從「戰鬥邏輯」中完美抽離。

VFRewardSubsystem: 管理 Rogue-lite 機制下的動態獎勵派發。
</details>


# 2. 前端革命：資料驅動的 UI 與 Common UI 實戰
開發初期最耗費心力，為解決「難以維護」與「手把熱切換焦點遺失」痛點而生。
<details>
<summary>點擊展開說明</summary>
  
## 2.1 MVVM 架構與 DataRegistry 實作
為了徹底告別混亂的 UI 藍圖 (Blueprint Spaghetti)，我採用了資料層與表現層分離的架構。所有 UI 所需的選單資料（如設定選項、玩家屬性）皆於 C++ 內部構建並封裝。
```C++
// 集中管理並構建 UI 資料的 Registry，介面僅負責監聽與渲染
UCLASS()
class VOIDFATE_API UOptionsDataRegistry : public UObject
{
    GENERATED_BODY()

public:
    // 初始化時，由 Registry 負責生成所有的 Data Object
    void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

    const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const { return RegisteredOptionsTabCollections; }
    TArray<UListDataObject_Base*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const;

private:
    // 透過遞迴演算法，高效檢索所有子層級的介面資料
    void FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const;
    
    // 將不同類型的設定分塊初始化，保證高擴充性
    void InitGameplayCollectionTab(ULocalPlayer* InOwningLocalPlayer);
    void InitAudioCollectionTab();
    void InitVideoCollectionTab();
    void InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer);
    void InitLanguageCollectionTab();

    UPROPERTY(Transient)
    TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;
};
```

## 2.2 效能優化：結合 CommonListView 的虛擬化渲染
構建出上述的 UListDataObject 後，我將其與 CommonListView 結合。
傳統 UMG 如果有 100 個設定選項，會同時生成 100 個 Widget，極度消耗記憶體。透過 CommonListView 的虛擬化 (Virtualization) 特性，系統僅會生成畫面上可見的 Entry Widget，並在玩家滾動時動態替換 Data Object，大幅降低了渲染開銷。

## 2.3 企業級體驗：設備熱切換 (Hot-Swapping) 與焦點追蹤
除了資料架構，本專案更解決了 UE 開發中最棘手的痛點：「手把與鍵鼠熱切換時的焦點遺失 (Focus Loss)」。

精準的路由控制：基於 Common UI 的 ActivatableWidgets 實作標準的 UI Stack 與 Back Handler (返回邏輯)。

多端適配：確保玩家在遊玩過程中，不論如何交替使用手把與滑鼠，系統都能精準追蹤並自動恢復最後一次操作的按鈕焦點，達到主機級遊戲 (Console-Like) 的流暢體驗。
<img width="1280" height="720" alt="VoidFate   2026-04-17 19-31-30" src="https://github.com/user-attachments/assets/139d318e-5149-4163-b6b8-142df11eb6f7" />

## 2.4 企劃友善的非同步 UI 節點 (Blueprint Async Actions)
在傳統的 UMG 藍圖開發中，呼叫一個「確認視窗 (Confirm Screen)」並等待玩家點擊「確定」或「取消」，往往需要綁定繁瑣的 Event Dispatcher，導致藍圖邏輯支離破碎。

為了解決這個痛點並提升企劃的開發體驗，我透過 C++ 繼承 UBlueprintAsyncActionBase，開發了專屬的非同步節點 (AsyncAction_PushConfirmScreen)。
```C++
// 宣告多播委派，用於藍圖節點的執行分支
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmScreenAction);

UCLASS()
class VOIDFATE_API UAsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    // 成功與取消的藍圖執行引腳 (Execution Pins)
    UPROPERTY(BlueprintAssignable)
    FOnConfirmScreenAction OnConfirmed;

    UPROPERTY(BlueprintAssignable)
    FOnConfirmScreenAction OnCancelled;

    // 靜態工廠函數，供藍圖呼叫並傳入視窗設定參數
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
    static UAsyncAction_PushConfirmScreen* PushConfirmScreen(UObject* WorldContextObject, TSubclassOf<UWidget_ConfirmScreen> ConfirmScreenClass);

    virtual void Activate() override;
};
```
</details>

# ⚔️ 3. 核心戰鬥架構：3A 級 Gameplay Ability System (GAS) 深度實作
《Void Fate》的戰鬥底層完全基於 Unreal Engine 的 Gameplay Ability System (GAS) 打造。本專案不單純只是使用基本的 GameplayAbility，而是深入自定義了 Task、MMC、ExecCalc 以及與 AI 行為樹的橋接，以實現高度的擴展性與極致的戰鬥手感。
<details>
<summary>點擊展開說明</summary>

## 3.1 資料驅動的初始化與 Rogue-like 動態擴展 (Data-Driven & Runtime Grants)
為了讓企劃能完全掌控數值與技能分配，專案徹底捨棄了 C++ 寫死的做法：

啟動資料 (Startup Data)：透過實作 DataAsset_StartUpDataBase (及其子類別 Ninja / Enemy)，所有角色在生成的瞬間，其初始屬性、預設 Gameplay Abilities (GA) 與 Gameplay Effects (GE) 都是透過 DataAsset 讀取並賦予。

Rogue-like 動態獎勵系統：結合自定義的 VFRewardSubsystem，玩家在遊戲過程中獲得的強化（如：攻擊附帶吸血、攻速提升等），本質上是在 Runtime 階段動態賦予全新的 GA 與 GE。這證明了本架構具備極高的動態擴展能力，能輕易支撐肉鴿遊戲的隨機性。

## 3.2 徹底解耦的 UI 屬性廣播 (Delegate Events)
在傳統做法中，UI 經常透過 Tick 去抓取玩家血量，這會造成極大的效能浪費與高耦合。
本專案在 VFAttributeSet 中實作了完整的 Delegate 廣播機制。當 GAS 內部的屬性（如 Health、Posture）發生變化時，會主動發送 Event。前端的 UI 組件僅需綁定這些 Delegate 即可被動更新，UI 系統完全不知道 GAS 的存在，實現了底層數值與前端視覺的完美解耦。
<img width="1280" height="720" alt="螢幕擷取畫面 2026-04-17 195945" src="https://github.com/user-attachments/assets/a451b071-6d7a-4dfe-a4fe-59b3f0a85649" />

## 3.3 突破壁壘：GAS 與 AI 行為樹的深度整合 (AI Target Lock-on)
這是本專案最具技術含量的挑戰之一。傳統 AI 行為樹難以精準控制複雜的技能邏輯，為此我開發了專屬的行為樹節點：

BTTask_ActivateAbilityWithTarget：這讓 AI 能夠直接從行為樹根據 Blackboard 的邏輯，精準觸發對應的 Gameplay Tag 來施放 GAS 技能。

動態目標追蹤 (Dynamic Target Tracking)：
為了解決敵方導彈或突進技能「無法死死咬住玩家」的痛點，我在此 Task 中，將玩家即時的 Actor 資訊封裝為 FGameplayAbilityTargetDataHandle，並透過 Event Payload 傳遞給 GA。

實戰效果：不論玩家如何靈活位移，敵方的追擊技能都能從 TargetData 中實時解算玩家座標，實現壓迫感極強的高難度追蹤攻擊。
https://github.com/user-attachments/assets/92e18d52-22bc-4ec7-929a-04ec0e14ba04

## 3.4 極致的打擊感與時機控制 (Anim Notify & Gameplay Events)
為了讓戰鬥的「判定幀」達到動作遊戲的極致精準，我拒絕使用粗糙的 Anim Notify State 碰撞：

Gameplay Event 驅動：透過自定義的 Anim Notify（如 VFAnimNotify_PostProcessFlash 等），在攻擊動畫最精確的打擊幀向 Ability System 發送 GameplayEvent。

GA 在收到 Event 後才正式觸發 GEExecCalc_DamageTaken 計算傷害。這確保了「視覺動作」與「底層數值計算」的絕對同步。

## 3.5 動態數值運算：MMC 與 ExecCalc 的進階應用
本專案的數值公式並非簡單的加減乘除，而是充分利用了 GAS 的進階運算類別：

客製化處決與傷害計算 (GEExecCalc_DamageTaken)：在 C++ 中攔截傷害事件，綜合計算護甲、架勢值 (Posture)、以及處決狀態，計算出最終的真實傷害。

自定義修飾符 (MMC_PostureRecoveryRate)：
為增加遊戲深度，實作了動態架勢恢復邏輯。透過 C++ 的 MMC (Modifier Magnitude Calculation)，即時抓取玩家的「當前血量比例」來動態影響「架勢恢復率」（例如：血量越低，架勢恢復越慢）。這種複雜的動態關聯，充分展現了對 GAS Pipeline 的掌握。

## 3.6 視覺與程式碼的交響樂：Custom Ability Tasks
為了讓 GameplayAbility 的腳本保持純粹的戰鬥邏輯，我將所有「非數值」的視覺回饋剝離，封裝成獨立的非同步任務 (Ability Tasks)：

AbilityTask_PlayCinematicCamera：專門處理處決或大招時的運鏡切換。

AbilityTask_ExecuteTaskOnTick 等：處理如殘影濾鏡、畫面震動 (Camera Shake) 或後處理 (Post-Process) 閃爍。

優勢：技能設計者 (企劃) 可以像拼圖一樣，在 GA 藍圖中串接這些 Task，既能保證視覺的華麗度，又不會污染核心的傷害運算邏輯。
  
</details>

# 4. 架構基石：高度解耦的 Component 模組化與 Interface 溝通
在許多傳統的 Unreal 專案中，開發者常犯的錯誤是將所有的戰鬥、UI 更新與輸入邏輯全部塞入 ACharacter 類別中，這會導致類別過度膨脹（God Class）且難以維護。《Void Fate》嚴格遵守單一職責原則 (SRP)，採用「組合優於繼承 (Composition over Inheritance)」的設計模式，將邏輯徹底抽離。
<details>
<summary>點擊展開說明</summary>

## 4.1 核心基底：PawnExtensionComponentBase
為了讓所有的自定義 Component 具備一致的生命週期與快取能力，我建構了 UPawnExtensionComponentBase 作為所有邏輯組件的底層。

快取機制 (Caching)：在此基底類別中，統一處理並快取 Owning Pawn 的指標，避免在 Runtime 頻繁呼叫 GetOwner() 與轉型 (Cast)，有效節省 CPU 開銷。

權限管理：統一管理伺服器與客戶端的權限驗證邏輯，為未來的多人連線 (Multiplayer) 架構打下擴展基礎。

## 4.2 邏輯容器化：Combat 與 UI Components
《Void Fate》的主角 (VFNinjaCharacter) 與敵人 (VFEnemyCharacter) 本身幾乎不包含具體的戰鬥或介面更新程式碼，它們更像是一個「容器」，透過掛載以下組件來獲得能力：

戰鬥組件 (PawnCombatComponent)：

專責處理武器的裝備與卸除 (VFWeaponBase)。

管理連擊狀態 (Combo States)、命中判定 (Hit Detection) 以及 GAS 的 Targeting 邏輯。

透過繼承衍生出 NinjaCombatComponent (處理玩家專屬的鎖定與拾取邏輯) 與 EnemyCombatComponent (處理 AI 專屬的武器邏輯)，兼顧了重用性與特化性。

UI 組件 (PawnUIComponent)：

專責處理「資料」到「前端介面」的橋接。當 GAS 的 Delegate 廣播血量或架勢值變動時，由這個 Component 接收並推播給 HUD，確保 Character 完全不依賴任何 UMG Widget。

## 4.3 終極解耦：Interface-Driven Communication (介面驅動開發)
這是我在專案中最引以為傲的架構設計之一。即便我們把邏輯拆到了 Component，如果其他系統（如 AnimNotify 或 AI）還是需要 Cast<AVFNinjaCharacter> 來獲取戰鬥組件，那就依然存在強耦合。

為徹底解決此問題，我導入了 Interface 驅動開發：

實作了 IPawnCombatInterface 與 IPawnUIInterface。

任何需要戰鬥邏輯的外部物件，都不需要知道它面對的是玩家還是敵人，只需要驗證該 Actor 是否實作了對應的 Interface，即可安全地取得 CombatComponent 進行操作。

```C++
UPawnCombatComponent* UVFFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UVFFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EVFValidType& OutValidType)
{
	UPawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

	OutValidType = PawnCombatComponent ? EVFValidType::Valid : EVFValidType::Invalid;

	return PawnCombatComponent;
}
```

## 4.4 架構帶來的開發優勢 (Architecture Benefits)
極速的企劃迭代：如果明天企劃要求新增一個「只有 UI 但不能戰鬥的 NPC」，我只需要給他掛載 PawnUIComponent，完全不用寫新類別，也不用擔心戰鬥代碼報錯。

安全的團隊協作：處理 UI 的工程師與處理戰鬥的工程師，可以分別在 PawnUIComponent.cpp 與 PawnCombatComponent.cpp 中工作，永遠不會發生 Git Merge 衝突 (Merge Conflicts)。
</details>

# 5. 視覺打磨與沉浸感：程式化運鏡與無縫載入架構
在動作遊戲中，極致的打擊感與視覺回饋是靈魂。然而，許多專案為了追求畫面張力，常將運鏡、濾鏡與 UI 邏輯硬塞入 Character 或 PlayerController 中，導致難以維護。《Void Fate》在視覺系統的實作上，嚴格遵守了邏輯抽離與非同步處理的業界標準。
<details>
	<summary>點擊展開說明</summary>

## 5.1 抽離攝影機邏輯：自定義 Camera Modifiers
當玩家的刀劍命中敵人時，我們需要「FOV 瞬間縮放」來營造衝擊力；當進入特殊狀態時，我們需要「全螢幕濾鏡 (Post-Process)」。為了不讓這些邏輯污染核心腳本，本專案深度利用了 Unreal Engine 的 UCameraModifier 系統。

VFCameraModifier_FOVImpact：專責處理動態 FOV 的衝擊與平滑還原。

VFCameraModifier_PostProcess：動態注入並管理後處理材質的權重與生命週期。

架構優勢：當戰鬥系統需要視覺回饋時，只需將這些 Modifier 動態注入 (Add Modifier) 到 PlayerCameraManager 中。這使得**「戰鬥運算」與「攝影機運鏡」徹底分離**，設計師可以輕鬆疊加多種運鏡效果而不必擔心邏輯衝突。

## 5.2 GAS 與視覺的完美橋接：運鏡專屬 Ability Tasks
在 GAS 的標準流程中，GameplayAbility (GA) 應專注於數值與狀態的流轉。為了在施放技能或處決時加入華麗的運鏡，我將視覺表現封裝成了專屬的非同步任務：

AbilityTask_PlayCinematicCamera：當觸發處決技能時，透過此 Task 呼叫 VFExecutionCameraSubsystem 接管主攝影機，播放預先設定的 Cinematic 運鏡。

企劃只需在技能藍圖中串接此 Task 節點，即可在精確的技能幀觸發電影級運鏡，完美兼顧了代碼潔癖與視覺華麗度。

## 5.3 企業級無縫體驗：非同步 Loading Screen Subsystem
除了戰鬥表現，遊戲的整體流暢度也至關重要。在關卡切換時，如果只使用預設的 OpenLevel，會導致主執行緒阻塞 (Thread Blocking)，畫面陷入死當或黑屏。

為了達到 3A 級別的無縫體驗，我實作了：

VFLoadingScreenSubsystem & VFLoadingScreenInterface：
在 C++ 系統層級攔截 Level 載入事件。利用非同步關卡載入 (Async Level Loading) 的特性，在背景載入資源的同時，於前端渲染高質感的過場 UI 與提示字卡，徹底消除玩家在等待時的焦慮感與出戲感。
</details>

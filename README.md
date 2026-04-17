# VoidFate-UE5-Action-Roguelite-Source
這是我花了四個月單人開發的專案。為了確保效能與可擴展性，我捨棄了純藍圖開發。 底層戰鬥我深入客製化了 GAS 的 ExecCalc 與 Ability Tasks； 系統架構我全面採用 Subsystem 與 Interface 避免硬引用； UI 層我利用 C++ Data Object 結合 CommonUI 實作虛擬化列表，徹底解決了 UI Tick 效能瓶頸。 這不僅是一款遊戲 Demo，更是一個以 3A 專案標準打造的系統架構雛形。

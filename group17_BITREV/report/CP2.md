# CP2 — RV32IM 實作

> 本檔格式依 HOMEWORK.md 第 5.3 節（六段式）。

## 1. 本次目標

以 RV32IM 組合語言實作 32-bit BITREV，完成靜態與動態指令數及分支數統計，
並以 CP1 的 11 組測資驗證輸出正確。

## 2. 設計內容

核心位於 [`../src/rv32im/bitrev.S`](../src/rv32im/bitrev.S)，介面為
`uint32_t bitrev(uint32_t x)`，輸入與輸出都使用 `a0`。

實作採用分治式位元交換：依序交換 16-bit、8-bit、4-bit、2-bit 區塊，
最後交換相鄰的單一位元，共 5 步。每一步以遮罩保留要移動的半邊，
再以移位與 OR 組合，因此不需要迴圈。

暫存器配置：`a0` 為輸入／輸出；`t0`、`t1` 存放兩組互補遮罩；
`t2`、`t3` 存放移位前後的中間值。核心是 leaf function，
不使用堆疊，也沒有條件分支；單次呼叫固定執行 36 條指令，
與輸入值無關。

同一檔案另含 Ripes 測試 harness（使用 Ripes 的 `ecall` 輸出與 CP1 測資）。
指令統計只計算 `bitrev` 核心，不包含 harness、列印與測試迴圈。

## 3. 結果與分析

以下區分 `bitrev` 核心與完整測試程式。核心統計範圍為
`bitrev` 標籤到 `ret`，不含呼叫端、測試迴圈與列印函式。

| 統計範圍 | 靜態指令數 | 動態指令數 | 靜態條件分支數 | 動態條件分支執行數 |
|---|---:|---:|---:|---:|
| `bitrev` 核心（單次呼叫） | 36（含 `ret`；不含 `ret` 為 35） | 36 | 0 | 0 |
| `bitrev` 核心（11 組測資各呼叫一次） | 36 | 396（36 × 11） | 0 | 0 |
| 完整測試程式（11 組測資） | — | 6487 | 不計入核心 | 不計入核心 |

**計數方法與工具**

- 靜態指令數：以 GNU RISC-V GCC 16.2.0 組譯，GNU Binutils `objdump`
  2.47.20260726 反組譯，目標參數為 `-march=rv32im -mabi=ilp32`；
  以 `bitrev` 到 `ret` 的機器碼逐條計數。
- 動態指令數：以 Ripes `v2.2.6-106-g5b8a616`、`RV32_5S`、RV32IM，
  透過 `--iret` 實測完整 11 組測試程式，得到 6487 條。
- 核心每次呼叫固定執行 36 條，11 次合計 396 條；
  此數由固定路徑與呼叫次數核算，非 Ripes 的函式區段計數。
- 正確性另以 Ripes `v2.3.0` 重跑確認，11/11 PASS；GNU RISC-V GCC
  14.2.0 交叉組譯亦通過。

**假設與說明**

- `ret` 展開為 `jalr`，計入指令數，但不列為條件分支。
- 靜態與動態的核心數字相同，因為核心沒有迴圈或條件分支。
- 完整程式的 6487 條包含測試 harness、列印與核心，不能用來代表核心成本。
- 本階段不要求 cycle 數。

**正確性驗證**

CP1 的 11 組測資（含 `0x00000000`、`0x80000000`、`0xFFFFFFFF` 等邊界值）
全部 PASS，Ripes 輸出 `PASSED=11 FAILED=0`。逐筆輸入／預期／實際結果見
[`cp2-b/test-results.md`](cp2-b/test-results.md)，截圖見
[`figures/`](figures/)。統計原始輸出與反組譯見
[`cp2-c/ripes-instruction-count.txt`](cp2-c/ripes-instruction-count.txt) 與
[`../src/rv32im/instruction-count-objdump.txt`](../src/rv32im/instruction-count-objdump.txt)。

## 4. 遭遇的問題與解法

一開始，測試紀錄中的 `source_sha256` 與實際要提交的 `bitrev.S`
不一致：紀錄顯示的檔案指紋對不上 repo 中的檔案。追查後發現，
測試是在本機另一份交付副本上執行，而不是 repository 中的檔案。

以 5 Whys 追問：

1. 為何 hash 不符？因為測試執行的不是 repo 中的 `bitrev.S`。
2. 為何會這樣？因為本機交付資料夾與 repo 各自存在一份檔案。
3. 為何會有兩份？因為流程先在本機整理，再上傳到 repo。
4. 為何沒有及早發現？因為沒有在重跑前核對來源檔案。
5. 為何需要核對？因為測試證據必須對應真正提交的程式碼。

解法是改用 repo 中的 `bitrev.S` 重新執行測試，並以
[`source-provenance.txt`](cp2-b/source-provenance.txt) 記錄來源 commit
與 SHA-256；另外加入 `.gitattributes` 固定 `bitrev.S` 使用 LF，
避免 Windows 換行造成 hash 改變。

## 5. 白話說明

BITREV 就是把 32 個位元的左右順序整個反過來，例如最低位跑到最高位。
我們的做法像把一疊牌對半交換：先交換左右各 16 位，再 8 位、4 位、2 位，
最後交換相鄰的 1 位，五步就完成，所以不需要迴圈，每次固定 36 條指令。
過程中最大的問題是測試的檔案和要交的檔案不是同一份，造成指紋對不上；
後來改成直接對 repo 裡的檔案重跑，並固定使用 LF 換行，才讓證據一致。

## 6. AI 使用說明

本組使用 opencode（AI 編碼助理）協助整理文件與檢查程式，並使用 Ripes 進行
模擬、GNU RISC-V GCC 進行編譯驗證。AI 產出的文字與數值均由組員逐一核對、
實際重跑後才寫入報告。

# group-17-BITREV

## 專案簡介

本專案實作 BITREV（位元反轉，DSP 類）指令。`BITREV` 將 32-bit 輸入
`rs1` 的位元順序完全反轉後寫回 `rd`，語意為：

```text
rd[i] = rs1[31 - i],  i = 0 … 31
```

正式語意與測試向量見 [HOMEWORK.md 第六節](../../HOMEWORK.md#六-題目語意規格)。

## 倉庫內容

| 路徑 | 內容 |
|---|---|
| `README.md` | 本檔 |
| `CONTRIBUTIONS.md` | 個人貢獻聲明 |
| `c_baseline/` | CP1 C baseline：核心函式、測資、測試、Makefile |
| `report/` | CP1 報告（六段式） |
| `spec/`、`references/`、`rv32im/`、`tests/` | CP2/CP3 預留 |

## CP1 重點

- 核心函式 `uint32_t bitrev(uint32_t x)`：分治式位元交換（swap 16/8/4/2/1，5 步、無迴圈）
- 11 組測資（含邊界值），以 naive 逐位元實作交叉比對
- `make run` 執行結果：**11/11 passed, 0 failed**

## 使用方式

```sh
cd c_baseline
make run     # 編譯並執行全部測資
make clean   # 清除執行檔
```

## 小組資訊

| 項目 | 內容 |
|---|---|
| 組名 | BITREV |
| 組長 | Fan Xin-Yu |
| 組員 | Fan Xin-Yu（1114405020）、Chen Jie-Xin（1114405042）、A1SimpleSmile（1114405041）、Allenyeh（1114405012） |
| 分支 | `dev/1114405020`、`dev/1114405042`、`dev/1114405041`、`dev/1114405012` |
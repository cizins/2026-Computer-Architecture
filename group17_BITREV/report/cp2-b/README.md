# report/cp2-b/

CP2 測試紀錄（任務 B：編譯與模擬）。

| 檔案 | 說明 |
|---|---|
| `test-results.md` | 11 組 CP1 測資的逐筆輸入／預期／實際結果與 PASS/FAIL 摘要 |
| `manifest.json` | 受測來源、工具版本、`source_sha256` 與測試統計 |
| `command.txt`、`cp1-command.txt` | Ripes 與 CP1 baseline 的執行命令 |
| `ripes-stdout.txt`、`ripes-stderr.txt`、`ripes-registers.txt` | Ripes CLI 原始輸出 |
| `cp1-baseline.txt`、`gcc-build.txt` | CP1 baseline 與 GCC 交叉組譯輸出 |
| `source-provenance.txt` | 受測 `bitrev.S` 的來源 commit 與 SHA-256 |

重現方式見 [`../../src/rv32im/BUILD_AND_SIMULATION.md`](../../src/rv32im/BUILD_AND_SIMULATION.md)。

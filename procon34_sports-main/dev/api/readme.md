# APIクラス
## 環境・ライブラリ
- g++ (Ubuntu 13.1.0-8ubuntu1~20.04.2) 13.1.0
- [nlohmann-json3](https://github.com/nlohmann/json)
```bash
$ sudo apt install nlohmann-json3-dev
```
- [libcurl](https://github.com/curl/curl)
```bash
$ sudo apt-get install libcurl4-openssl-dev
```
※openssl版以外は動作未確認
## 使用例
```cpp Main.cpp
int main(){
    // 行動計画(仮)
    vector<vector<int>> ac = {{1,1},{1,4}};
    // 先攻
    API a = API("1","10");
    // 後攻
    API b = API("2","10");
    
    // 試合一覧を取得
    cout << a.getMatchesInfo() << endl;
    // 試合id10の試合情報を取得
    cout << a.getMatchInfo() << endl;
    
    // 行動計画をjsonに変換
    json sendData = a.convertJson(ac,1);
    // 行動計画を送信
    cout << a.postActionPlan(sendData) << endl;    
}
```
### コンパイル
```
$ g++ main.cpp -lcurl
```

## 出力結果
### 試合一覧を取得
#### 成功時
```json 成功時
{
    "matches": [
        {
            "board": {
                "height": 11,
                "mason": 2,
                "masons": [
                    [
                        0
                    ]
                ],
                "structures": [
                    [
                        0
                    ]
                ],
                "width": 11
            },
            "bonus": {
                "castle": 100,
                "territory": 30,
                "wall": 10
            },
            "first": true,
            "id": 10,
            "opponent": "後攻チーム",
            "turnSeconds": 5,
            "turns": 30
        }
    ],
    "message": "succeeded",
    "status_code": 0
}

```
#### 失敗時
```json
{
    "message": "failed",
    "status_code": 28
}
```

--- 

### 試合情報取得
#### 成功時
```json
{
    "board": {
        "height": 11,
        "mason": 2,
        "masons": [
            [
               0
            ]
        ],
        "structures": [
            [
                0
            ]
        ],
        "territories": [
            [
                0
            ]
        ],
        "walls": [
            [
                0
            ]
        ],
        "width": 11
    },
    "id": 10,
    "logs": [],
    "message": "succeeded",
    "status_code": 0,
    "turn": 0
}
```
#### 失敗時
```json
{
    "message": "failed",
    "status_code": 28
}
```
---
### 行動計画送信
#### 成功時
```json
{
    "accepted_at": 1695990218182,
    "message": "succeeded",
    "status_code": 0
}
```
#### 失敗時
```json
{
    "message": "failed",
    "status_code": 28
}
```

## フィールド
※ 全てprivate
| 変数名・型       | 内容                           | 
| -------------| ---------------------------| 
| `string token`   | トークン                       | 
| `string id`      | 現在の試合id                   | 
| `string baseURL` | 基となるURL                     | 
| `json postData` | 受理された行動計画             | 
| `int UNIXTIME`   | 行動計画受理時のUNIXTIMEを保存 | 
| `int TIMEOUT`   | タイムアウト度時間 |
| `int turn`   | 現在のターンの一つ前 |  
## メソッド

### `API(string token,string id)`
コンストラクタ
#### 引数
`string token`:トークン
`string id`:試合id
### 処理内容
トークンを引数で初期化
idを引数で初期化
turnを-1で初期化
baseURLを初期化

---

### `getTimeStamp()`
#### 引数
なし
#### 戻り値
- `String ss` :HHMMSS(24h)
#### 処理内容
現在時刻を取得し文字列として返す
log記録のために使用

---

### `WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)`

#### おまじない

---

### `get(string url)`
#### 引数 
- `String URL`
#### 戻り値
- `json j`:(取得結果 + ステータスコード)

#### 処理内容
url で指定されたサイトからgetし，jsonデータを返す

---

### `post(json post_data,string url)`
#### 引数
- `json post_data` : postするjsonデータ
- `string url` : post先のリンク
#### 戻り値
- `json j`:responseを返す
#### 処理内容
urlで指定されたサイトにjsonファイルをポストし，そのレスポンスを返す.


---

### `saveAsJson(json j,string pass,string filename)`
#### 引数
- `json j`:セーブするデータ
- `string pass`:保存先
- `string filename`:ファイル名
#### 戻り値
なし
#### 処理内容
与えられたjsonデータを指定された場所に保存する

---

### `makeDir(string name,string pass)`
#### 引数
- `string name`:ディレクトリ名
- `string pass`:作成場所
#### 戻り値
- `bool result`:新たに作成されたかどうか
#### 処理内容
ディレクトリの作成

---

### `changeID()`
#### 引数
- `string id`:変更後のid
#### 戻り値
なし
#### 処理内容
IDのsetter

---

### `getID()`
#### 引数
なし
#### 戻り値
- `string id` : 現在の試合id
#### 処理内容
現在の試合idを返す

---

### `getTrunFromWeb()`
#### 引数
なし
#### 戻り値
- `int turn` : 現在の試合idでのターン数(取得できなかった場合 -1 を返す)
#### 処理内容
現在の試合idでのターン数を**サーバー上から取得し**返す

---
### `setTrun()`
#### 引数
- `int t` :変更後のターン数
#### 戻り値
なし
#### 処理内容
ターン数を変更する．

---

### `getMatchesInfo()`
#### 引数
なし
#### 戻り値
- json j:試合の一覧
#### 処理内容
試合の詳細をgetし，jsonで返す．
この時，そのファイルが `MatchesInfo-HHMMSS.json`で保存される

#### 保存内容
##### 成功時
```json 成功時
{
    "matches": [
        {
            "board": {
                "height": 11,
                "mason": 2,
                "masons": [
                    [
                        0
                    ]
                ],
                "structures": [
                    [
                        0
                    ]
                ],
                "width": 11
            },
            "bonus": {
                "castle": 100,
                "territory": 30,
                "wall": 10
            },
            "first": true,
            "id": 10,
            "opponent": "後攻チーム",
            "turnSeconds": 5,
            "turns": 30
        }
    ],
    "message": "succeeded",
    "status_code": 0
}

```
##### 失敗時
```json
{
    "message": "failed",
    "status_code": 28
}
```
---

### `getMatcheInfo()`
#### 引数
なし
#### 戻り値
- json j:試合idの状況
#### 処理内容
現在の試合の状況を取得しjsonデータで返す．
この時， `./{試合id}`ディレクトリに `{turn}.json`のファイル名で保存される
#### 保存内容
##### 成功時
```json
{
    "board": {
        "height": 11,
        "mason": 2,
        "masons": [
            [
               0
            ]
        ],
        "structures": [
            [
                0
            ]
        ],
        "territories": [
            [
                0
            ]
        ],
        "walls": [
            [
                0
            ]
        ],
        "width": 11
    },
    "id": 10,
    "logs": [],
    "message": "succeeded",
    "status_code": 0,
    "turn": 0
}
```
##### 失敗時
```json
{
    "message": "failed",
    "status_code": 28
}
```
---
### `postActionPlan(json j)`
#### 引数
- json j :送る職人ごとの行動計画
#### 戻り値
- json res:成功の可否

#### 処理内容
行動計画を現在の試合idへと送信する


---

### `convertJson(vector<vector<int>> ActionPlan,int next_turn)`
#### 引数
- `vector<vector<int>> ActionPlan` : 
2×職人の人数分の多次元配列\
$i$番の職人の行動計画は `ActionPlan[i]`にあり，
    -    `ActionPlan[i][0]`:
    行動タイプ(0: 滞在, 1: 移動, 2: 建築, 3: 解体)
    - `ActionPlan[i][1]`:
    行動方向

- `int next_trun` :送信したいターン数

#### 戻り値
- json j:jsonに変換した行動計画

#### 処理内容
2次元配列をjsonファイルに変換する
#### 保存内容
##### 成功時
```json
{
    "accepted_at": 1695990218182,
    "message": "succeeded",
    "status_code": 0
}
```
##### 失敗時
```json
{
    "message": "failed",
    "status_code": 28
}
```
---
## ステータスコード(`status_code`)一覧
|コード|message|内容|
|----|----|---|
|-100|TooEarly|競技開始前です|
|-99||接続エラー|
|0|suceeded|処理に成功|
エラー発生時はエラーメッセージが表示される．
全て：https://curl.se/libcurl/c/libcurl-errors.html
API仕様：https://www.procon.gr.jp/wp-content/uploads/2023/07/spec.html
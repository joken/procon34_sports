# チュートリアル
githubとgitを使ってチーム開発する時の軽いガイド的なやつ
リモート(リポジトリ)をgithub，ローカル(リポジトリ)をgitで管理する

## ローカルにコピーする
ssh : `$ git clone -b develop  git@github.com:joken/procon34_sports.git`
https : `$ git clone -b develop  https://github.com/joken/procon34_sports.git`
`$ cd ./procon34_sports

## ローカルのブランチを最新に更新する
`$ git pull`

## ブランチを分ける
開発を始める前に作業履歴を分けるために作業内容ごとにブランチを分ける
`$ git branch feature/[new branch name]`
`$ git switch feature/[new branch name]`

> `$ git switch -c feature/[new branch name]`で作成と移動を同時にできたはず
`
そのあとはローカル環境でコードとかの作成，変更する

## ローカルでの更新
ローカル内での作業ではこれを定期的に回すのが基本

1. 変更を加える
2. 追加したファイルとかをgitに追加 : `$ git add .`
3. gitに追加したファイルを登録(コミット) : `$ git commit`

> コミットは `$ git commit -m "メッセージ"`とするとコミットメッセージもコマンド上で書ける

## リモートリポジトリへpushする
作業しているブランチが煮詰まったらリモートに変更を反映させる
`$ git push main feature/[new breanch name]`


## プルリクエストを送る(あまり経験ないから内容が怪しい)
ブラウザでGithub上のリポジトリページに飛ぶ
画面上部の[Compare & pull request]を押して編集作成していく
1. レビュアーの指定(なくてもいける？)
2. タイトルに`feature/[new branch name] 内容の要約文`
3. 概要や問題点とかを記載
4. [Create pull request]を押して投稿

## その他gitのコマンド
- gitの状態確認 : `$ git status`
- ログの確認 : `$ git log`
    * それぞれのログを1行にする : `$ git log --oneline`
- ブランチの一覧取得 : `$ git branch`

- 直前のコミットを取り消し(状態は据え置き) : `$ git reset --soft HEAD^`
- 直前のコミットを取り消し(状態も巻き戻す) : `$ git reset --hard HEAD^`
- コミットの打ち消し(状況を巻き戻すがログは残す) : `$ git revert [コミットのハッシュ値]`


---

# 参考文献
> チーム開発の流れ: https://qiita.com/yukiya1006/items/4a491df3595662d8f781
> コミットの取り消し: https://qiita.com/shuntaro_tamura/items/06281261d893acf049ed

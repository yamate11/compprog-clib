修正とテストの方法:

フィーチャーブランチに切り替える:
  cd ~/compprog-clib
  git checkout -b dev     (-b = ブランチを作る)
修正とテストを行う
コミットする
  git add 修正したファイルたち
  git commit -m "comment"
master ブランチにマージして，リモートに送る
  git checkout master
  git merge dev
  git push
フィーチャーブランチを削除する
  git branch -d dev

compprog で修正を取り込む
  cd ~/compprog/clib
  git pull

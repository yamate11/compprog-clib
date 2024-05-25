修正とテストの方法:

develop ブランチに切り替える:
  git checkout develop
修正とテストを行う
コミットする
  git add ....
  git commit -m "comment"
master ブランチにマージして，リモートに送る
  git checkout master
  git merge develop
  git push

compprog で修正を取り込む
  git submodule update
  

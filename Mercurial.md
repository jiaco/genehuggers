# Introduction #

Since I am very bad with RCS, this should help me remember how to do stuff with hg.

# Notes #

cd genehuggers
hg init -- FIRST TIME ONLY

creates genehuggers/.hg directory

in genehuggers/.hg/hgrc add this info:
```
[ui]
username = Me A. I <meAndI@gmail.com>
[paths]
default-push = https://genehuggers.googlecode.com/hg/
```

in genehuggers/.hgignore
```
syntax: glob
*.o
moc_*
Makefile
```

hg add -- to add in files

hg commit -- to commit files (local)

hg push -- to send files to google code

One day, someone pushed without first pulling. Then I could not push my changes, the quick and dirty workaround was to create a new branch
```
hg branch master
hg commit -m "Master branch created"
hg push -f
```
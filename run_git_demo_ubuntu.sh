#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

repo=git_demo_ubuntu
if [[ -e "$repo" ]]; then
  echo "Refusing to overwrite $repo" >&2
  exit 1
fi
mkdir "$repo"
cd "$repo"
git init -q
git config user.name 'Jingxin Zhou'
git config user.email 'student@example.invalid'
printf 'Jingxin Zhou\nstudent@example.invalid\n' > me.txt
git add me.txt
git commit -qm 'Add student details'

git checkout -qb collaborator
printf 'Imaginary Collaborator\ncollaborator@example.invalid\n' > me.txt
git add me.txt
git commit -qm 'Change details on branch'

git checkout -q master
printf 'Course VNAV Lab 1\n' >> me.txt
git add me.txt
git commit -qm 'Add course name'

if git merge collaborator; then
  echo 'Expected a content conflict, but the merge succeeded.' >&2
  exit 1
fi
cp me.txt ../git_conflict_ubuntu.txt
printf 'Jingxin Zhou\nstudent@example.invalid\nCourse VNAV Lab 1\n' > me.txt
git add me.txt
git commit -qm 'Resolve merge conflict'
git log --graph --oneline --all > ../git_log_ubuntu.txt
git diff HEAD~2 > ../git_diff_ubuntu.txt
test -z "$(git status --porcelain)"
cat ../git_log_ubuntu.txt

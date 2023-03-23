if [ $# != 1 ]
then
    echo "wrong number of parameters"
else
    git add .gitlab-ci.yml
    git commit -m $1
    git push
fi

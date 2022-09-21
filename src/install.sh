DIR="./out"
if [ -d "$DIR" ]; then
    rm -rf ./out
fi

mkdir -p out/build

cmake -S . -B out/build





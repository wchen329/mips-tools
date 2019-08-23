ls ../tests/*.cpp | sed s/.cpp/.o/ | sed s/"..\/tests\/"/" "/ -l 100 | tr -d \\n

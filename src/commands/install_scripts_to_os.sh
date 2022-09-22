if [ -d "./out/build" ]
then
	if [ -d "../out/build/bin" ]
	then
		cp ./out/build/* ../out/build/bin;
		echo "Install sucessful!";
	else
		echo "You need install the Aurora5-OS first!";
	fi
else
	echo "Use need run build_all_script.sh first!";
fi

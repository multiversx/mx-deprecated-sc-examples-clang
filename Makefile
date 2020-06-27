.PHONY: prepare

prepare:
	rm -rf ./erc20-c/elrond && cp -r ./elrond_api_c ./erc20-c/elrond
	rm -rf ./queue/elrond && cp -r ./elrond_api_c ./queue/elrond
	rm -rf ./simple-counter/elrond && cp -r ./elrond_api_c ./simple-counter/elrond
	rm -rf ./ultimate-answer/elrond && cp -r ./elrond_api_c ./ultimate-answer/elrond

	printf "{}\n" > ./erc20-c/elrond.json
	printf "{}\n" > ./queue/elrond.json
	printf "{}\n" > ./simple-counter/elrond.json
	printf "{}\n" > ./ultimate-answer/elrond.json
	
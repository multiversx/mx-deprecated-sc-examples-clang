.PHONY: prepare

prepare:
	rm -rf ./erc20-c/elrond && cp -r ./elrond_api_c ./erc20-c/elrond
	rm -rf ./queue/elrond && cp -r ./elrond_api_c ./queue/elrond
	rm -rf ./simple-counter/elrond && cp -r ./elrond_api_c ./simple-counter/elrond
	rm -rf ./ultimate-answer/elrond && cp -r ./elrond_api_c ./ultimate-answer/elrond
	rm -rf ./empty/elrond && cp -r ./elrond_api_c ./empty/elrond
	rm -rf ./moneybox/elrond && cp -r ./elrond_api_c ./moneybox/elrond

	
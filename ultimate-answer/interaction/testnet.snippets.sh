ALICE="${USERS}/alice.pem"
ADDRESS=$(erdpy data load --key=address-testnet)
DEPLOY_TRANSACTION=$(erdpy data load --key=deployTransaction-testnet)
PROXY=https://testnet-api.elrond.com

deploy() {
    erdpy --verbose contract deploy --project=${PROJECT} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --send --outfile="deploy-testnet.interaction.json" --proxy=${PROXY} --chain=T  || return

    TRANSACTION=$(erdpy data parse --file="deploy-testnet.interaction.json" --expression="data['emittedTransactionHash']")
    ADDRESS=$(erdpy data parse --file="deploy-testnet.interaction.json" --expression="data['contractAddress']")

    erdpy data store --key=address-testnet --value=${ADDRESS}
    erdpy data store --key=deployTransaction-testnet --value=${TRANSACTION}

    echo ""
    echo "Smart contract address: ${ADDRESS}"
}

checkDeployment() {
    erdpy tx get --hash=$DEPLOY_TRANSACTION --omit-fields="['data', 'signature']" --proxy=${PROXY}
    erdpy account get --address=$ADDRESS --omit-fields="['code']" --proxy=${PROXY}
}

getUltimateAnswer() {
    erdpy --verbose contract query ${ADDRESS} --function="getUltimateAnswer" --proxy=${PROXY}
}

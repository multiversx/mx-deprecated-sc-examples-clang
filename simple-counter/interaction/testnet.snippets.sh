ALICE="${USERS}/alice.pem"
ADDRESS=$(erdpy data load --key=address-testnet)
DEPLOY_TRANSACTION=$(erdpy data load --key=deployTransaction-testnet)
PROXY=https://testnet-api.elrond.com

deploy() {
    erdpy --verbose contract deploy --project=${PROJECT} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --send --outfile="deploy-testnet.interaction.json" --proxy=${PROXY} --chain=T

    TRANSACTION=$(erdpy data parse --file="deploy-testnet.interaction.json" --expression="data['result']['hash']")
    ADDRESS=$(erdpy data parse --file="deploy-testnet.interaction.json" --expression="data['emitted_tx']['address']")

    erdpy data store --key=address-testnet --value=${ADDRESS}
    erdpy data store --key=deployTransaction-testnet --value=${TRANSACTION}

    echo ""
    echo "Smart contract address: ${ADDRESS}"
}

checkDeployment() {
    erdpy tx get --hash=$DEPLOY_TRANSACTION --omit-fields="['data', 'signature']"
    erdpy account get --address=$ADDRESS --omit-fields="['code']" --proxy=${PROXY}
}

increment() {
    erdpy --verbose contract call ${ADDRESS} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --function="increment" --send --proxy=${PROXY} --chain=T
}

decrement() {
    erdpy --verbose contract call ${ADDRESS} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --function="decrement" --send --proxy=${PROXY} --chain=T
}

get() {
    erdpy --verbose contract query ${ADDRESS} --function="get" --proxy=${PROXY}
}

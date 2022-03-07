ALICE="${USERS}/alice.pem"
ADDRESS=$(erdpy data load --key=address-localnet)
DEPLOY_TRANSACTION=$(erdpy data load --key=deployTransaction-localnet)

deploy() {
    erdpy --verbose contract deploy --project=${PROJECT} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --send --outfile="deploy-localnet.interaction.json"  || return

    TRANSACTION=$(erdpy data parse --file="deploy-localnet.interaction.json" --expression="data['emittedTransactionHash']")
    ADDRESS=$(erdpy data parse --file="deploy-localnet.interaction.json" --expression="data['contractAddress']")

    erdpy data store --key=address-localnet --value=${ADDRESS}
    erdpy data store --key=deployTransaction-localnet --value=${TRANSACTION}

    echo ""
    echo "Smart contract address: ${ADDRESS}"
}

deploySimulate() {
    erdpy --verbose contract deploy --project=${PROJECT} --recall-nonce --pem=${ALICE} --gas-limit=1500000 --simulate --outfile="simulate-localnet.interaction.json"

    TRANSACTION=$(erdpy data parse --file="simulate-localnet.interaction.json" --expression="data['result']['hash']")
    ADDRESS=$(erdpy data parse --file="simulate-localnet.interaction.json" --expression="data['contractAddress']")
    RETCODE=$(erdpy data parse --file="simulate-localnet.interaction.json" --expression="data['result']['returnCode']")
    RETMSG=$(erdpy data parse --file="simulate-localnet.interaction.json" --expression="data['result']['returnMessage']")

    echo ""
    echo "Simulated transaction: ${TRANSACTION}"
    echo "Smart contract address: ${ADDRESS}"
    echo "Deployment return code: ${RETCODE}"
    echo "Deployment return message: ${RETMSG}"
}

checkDeployment() {
    erdpy tx get --hash=$DEPLOY_TRANSACTION --omit-fields="['data', 'signature']"
    erdpy account get --address=$ADDRESS --omit-fields="['code']"
}

increment() {
    erdpy --verbose contract call ${ADDRESS} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --function="increment" --send
}

decrement() {
    erdpy --verbose contract call ${ADDRESS} --recall-nonce --pem=${ALICE} --gas-limit=5000000 --function="decrement" --send
}

get() {
    erdpy --verbose contract query ${ADDRESS} --function="get"
}

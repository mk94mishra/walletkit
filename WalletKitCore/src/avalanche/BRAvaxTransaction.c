//
//  File.c
//  
//
//  Created by Amit on 12/08/2021.
//

#include "BRAvaxTransaction.h"

extern void avaxCreateBaseTx(){
   
//
    
    struct TransferableOutputRecord * outputs = calloc (1, sizeof(struct TransferableOutputRecord));
    struct SECP256K1TransferOutputRecord * secpOutput = calloc(1, sizeof(struct SECP256K1TransferOutputRecord));
    
    struct TranferableInputRecord * inputs = calloc(1, sizeof(struct TranferableInputRecord));
    
    struct BaseInputRecord * secInput = calloc(1, sizeof(struct BaseInputRecord));
    
    inputs->inputs_len = 1;
    inputs->inputs = calloc(1, sizeof(struct BaseInputRecord *));
    inputs->inputs[0] = secInput;
    secInput->type_id = SECP256K1TransferInput;
    
    
    free(outputs);
    free(inputs);
    free(secInput);
    free(secpOutput);
    
//    BRAvalancheTransferOutputBase tout = calloc(1, sizeof(struct BRAvalancheTransferOutputBaseRecord));
//    tout->target.amount = 101;
//
//
//    SECP256K1TransferOutput output1 = calloc(1, sizeof(struct SECP256K1TransferOutputRecord));
//    *output1 = SECP256K1TransferOutput_DEFAULT;
//    output1->amount = 666;
//
//
//    BRAvalancheBaseTx tx = calloc(1, sizeof(struct BRAvalancheBaseTxRecord));
//    *tx = BRAvalancheBaseTx_DEFAULT;
//    tx->outputs = calloc(1, sizeof(struct BRAvalancheTransferableOutputRecord) );
//    tx->outputs[0].outputlen =2 ;
//    tx->outputs[0].output = calloc(2, sizeof(struct BRAvalancheTransferOutputBaseRecord));
//    tx->outputs[0].output[0].type_id = 0xa;
//    tx->outputs[0].output[0].target.amount = 666;
//    tx->outputs[0].output[0].type=BRAvalancheSECP256K1TransferOutput;
//    tx->outputs[0].output[1].type_id = 0xb;
//    tx->outputs[0].output[1].target.amount = 808;
//    tx->outputs[0].output[1].type=BRAvalancheSECP256K1OutputOwners;
//
//    for(int i=0; i < tx->outputs[0].outputlen; i++){
//        printf("%d - %d \r\n", tx->outputs[0].output[i].type, tx->outputs[0].output[i].target.amount);
//        BRAvalancheTransferOutputType type = tx->outputs[0].output[i].type;
//        switch(type){
//            case BRAvalancheSECP256K1TransferOutput: printf("got a transfer \r\n"); break;
//            case BRAvalancheSECP256K1OutputOwners: printf("owner swap\r\n");
//                break;
//            default: printf("!!!unknown transfer type!!!\r\n"); break;
//        }
//
//    }
//
//    free(tx->outputs[0].output);
//    free(tx->outputs);
//    free(tx);
//    free(tout);
//    free(output1);
}


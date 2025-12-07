#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/GSprite.h"
#include "/home/codeleaded/System/Static/Library/NeuralNetwork.h"

#define SPRITE_PATH         "/home/codeleaded/Data/NN/Digits/"
#define DATA_PATH           "/home/codeleaded/Data/NN/DigitsGray/"
#define SPRITE_TEST         "testing"
#define SPRITE_TRAINING     "training"
#define SPRITE_COUNT        4
#define SPRITE_MAX          300

#define NN_PATH             "./data/Model.nnalx"
#define NN_COUNT            10
#define NN_LEARNRATE        0.1f


int epoch = 0;
int reality = 0;
int prediction = 0;
NeuralType loss = 0.0f;
GSprite sp;
AlxFont font;
NeuralNetwork nnet;

void NeuralNetwork_Render(unsigned int* Target,int Width,int Height,NeuralNetwork* nn){
    for(int i = 0;i<nn->layers.size;i++){
        NeuralLayer* nl = (NeuralLayer*)Vector_Get(&nn->layers,i);
        
        for(int j = 0;j<nl->count;j++){
            const int dx = 400.0f;
            const int x = i * dx;
            const int y = j * font.CharSizeY * 3;

            Rect_RenderXX(Target,Width,Height,x,y,100.0f,font.CharSizeY * 2,GREEN);
            
            String str = String_Format("%f",nl->values[j]);
            CStr_RenderSizeAlxFont(Target,Width,Height,&font,str.Memory,str.size,x,y,GRAY);
            String_Free(&str);
        
            if(nl->precount > 0){
                str = String_Format("%f",nl->biases[j]);
                CStr_RenderSizeAlxFont(Target,Width,Height,&font,str.Memory,str.size,x,y + font.CharSizeY,GRAY);
                String_Free(&str);
            }
        
            const int max = 3;
            const int count = nl->precount < max ? nl->precount : max;
            
            for(int k = 0;k<count;k++){
                if(nl->weights && nl->weights[j]){
                    str = String_Format("%f",nl->weights[j][k]);
                    CStr_RenderSizeAlxFont(Target,Width,Height,&font,str.Memory,str.size,x - dx * 0.5f,y + k * font.CharSizeY,GRAY);
                    String_Free(&str);
                }
            }
        }
    }
}

void Setup(AlxWindow* w){
    RGA_Set(Time_Nano());

    sp = GSprite_Null();
    font = AlxFont_MAKE_HIGH(12,24);
    
    nnet = NeuralNetwork_Make((NeuralLayerBuilder[]){
        NeuralLayerBuilder_Make(784,"relu"),
        NeuralLayerBuilder_Make(16,"relu"),
        NeuralLayerBuilder_Make(NN_COUNT,"softmax"),
        NeuralLayerBuilder_End()
    });
}
void Update(AlxWindow* w){
    if(Stroke(ALX_KEY_Q).PRESSED){
        NeuralNetwork_Save(&nnet,NN_PATH);
        printf("[NeuralNetwork]: Save -> Success!\n");
    }else if(Stroke(ALX_KEY_E).PRESSED){
        if(Files_isFile(NN_PATH)){
            NeuralNetwork_Free(&nnet);
            nnet = NeuralNetwork_Load(NN_PATH);
            printf("[NeuralNetwork]: Load -> Success!\n");
        }else{
            printf("[NeuralNetwork]: Load -> Failed!\n");
        }
    }else if(Stroke(ALX_KEY_W).PRESSED){
        NeuralDataMap ndm = NeuralDataMap_Make_GSprite(DATA_PATH SPRITE_TRAINING,&epoch,NN_COUNT,SPRITE_COUNT,SPRITE_MAX);
        NeuralNetwork_Learn(&nnet,&ndm,NN_LEARNRATE);
        NeuralDataMap_Free(&ndm);

        ndm = NeuralDataMap_Make_GSprite(DATA_PATH SPRITE_TEST,&epoch,NN_COUNT,SPRITE_COUNT,SPRITE_MAX);
        loss = NeuralNetwork_Test_C(&nnet,&ndm);
        NeuralDataMap_Free(&ndm);
    }else if(Stroke(ALX_KEY_S).PRESSED){
        unsigned int ndir = Random_u32_MinMax(0,10);
        unsigned int item = Random_u32_MinMax(0,SPRITE_MAX);

        NeuralDataPair ndp = NeuralDataPair_Make_GSprite(DATA_PATH SPRITE_TEST,ndir,item,NN_COUNT);
        loss = NeuralNetwork_Test(&nnet,&ndp);
        NeuralDataPair_Free(&ndp);

        prediction = NeuralNetwork_Decision(&nnet);
        reality = ndir;

        CStr ntraining_s = CStr_Format("%s/%d/%d.sprg",DATA_PATH SPRITE_TEST,ndir,item);
        GSprite_Free(&sp);
        sp = GSprite_Load(ntraining_s);
        CStr_Free(&ntraining_s);
    }

    Clear(DARK_BLUE);

    GSprite_Render(WINDOW_STD_ARGS,&sp,GetWidth() - sp.w - 50.0f,0.0f);

    NeuralNetwork_Render(WINDOW_STD_ARGS,&nnet);

    //String str = String_Format("T:%d, ND:%d, I:%d",test,ndir,item);
    //RenderCStrSize(str.Memory,str.size,0.0f,0.0f,WHITE);
    //String_Free(&str);

    String str = String_Format("Loss: %f, Is: %d, Pre: %d, -> %s",loss,reality,prediction,(reality == prediction ? "correct" : "wrong"));
    RenderCStrSize(str.Memory,str.size,0.0f,GetHeight() - GetAlxFont()->CharSizeY,WHITE);
    String_Free(&str);
}
void Delete(AlxWindow* w){
    NeuralNetwork_Free(&nnet);
    GSprite_Free(&sp);
    AlxFont_Free(&font);
}

int main(){
    if(Create("RGB to G",1920,1080,1,1,Setup,Update,Delete))
        Start();
    return 0;
}
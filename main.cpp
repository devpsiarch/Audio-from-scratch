#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;

//RIFF chunk descriptor 
const string chunk_id = "RIFF";
const string chunk_size = "----";
const string format = "WAVE";

//FMT sub-chunk (descipts the sounds data format)
const string subchunk1id = "fmt ";
const int subchunk1size = 16;
const int audioformat = 1;
const int numchannels = 2;
const int samplerate = 44100;
const int byterate = samplerate * numchannels * (subchunk1size/8);
const int blockalign = numchannels * (subchunk1size/8);
const int bitspersample = 16;

//DATA sub-chunk (contains size of data ans accual sounds ) 
const string subchunk2id = "data";
const string subchunk2size = "----";

// I made the choise for the duration variable to be chosen when calling the write methode 
// this value is gotten from this ==> 2^15 = 32760
const int max_amplitude = 32760;
const double frequency = 250;


void write_as_bytes(ofstream &file,int value,int byte_size){
    file.write(reinterpret_cast<const char*>(&value),byte_size);
}

int write_wav(string filepath,int duration){
    ofstream wav;
    wav.open(filepath,ios::binary);

    if(wav.is_open()){
        //writting the header stuff to the file
        wav << chunk_id;
        wav << chunk_size;
        wav << format;

        wav << subchunk1id;
        write_as_bytes(wav,subchunk1size,4);
        write_as_bytes(wav,audioformat,2);
        write_as_bytes(wav,numchannels,2);
        write_as_bytes(wav,samplerate,4);
        write_as_bytes(wav,byterate,4);
        write_as_bytes(wav,blockalign,2);
        write_as_bytes(wav,bitspersample,2);

        wav << subchunk2id;
        wav << subchunk2size;

        //writting audio data
        //has to be changed to somthing the user might wanna record
        int start_audio = wav.tellp();
        for(int i = 0 ; i < samplerate * duration;i++){
            double amplitude = (double)i / samplerate * max_amplitude;
            //Sound is all and all just sin ans cos waves
            double value = sin((2*3.14 * i * frequency)/samplerate);

                        
            //well define 2 channels cuz i set the numchannels to be 2 
            double channel1 = amplitude * value / 2;                    // will rise 
            double channel2 = (max_amplitude - amplitude)*value;    // will fall
            

            //writing is sequential
            write_as_bytes(wav,channel1,2);
            write_as_bytes(wav,channel2,2);
        }
        int end_audio = wav.tellp();
        //writing what we havnt done already
        wav.seekp(start_audio - 4);
        write_as_bytes(wav,end_audio - start_audio,4);

        wav.seekp(4,ios::beg);
        write_as_bytes(wav,end_audio - 8,4);

        wav.close();
        return 0;
    }else{
        return -1;
    }
}

int main(){

    write_wav("audio/test.wav",2); 
    return 0;
}

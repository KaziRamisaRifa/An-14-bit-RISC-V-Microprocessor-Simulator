#include <bits/stdc++.h>

using namespace std;

#define checkBit(n,p) (bool)(n&(1<<p))

/*============================================================================//
                       while(noSuccess)
                       {
                            tryAgain();
                            if(dead)
                            {
                                break;
                            }
                       }
//============================================================================*/

map<string,string> getMap(string path)
{
    string word,value;
    map<string,string> m;
    ifstream file(path);
    while(file>>word>>value)
    {
        m[word] = value;
    }
    file.close();
    return m;
}

string decToBin(int num,int length)
{
    string result = "";
    for(int i=0; i<length; i++)
    {
        if(checkBit(num,i))
        {
            result = "1" + result;
        }
        else
        {
            result = "0" + result;
        }
    }
    return result;
}

void BinarytoHex(string s,ofstream &out)
{
    int hexadecimalval = 0, i = 1, remainder;

    istringstream buffer(s);
    long long int binaryval;
    buffer >>binaryval;


    while (binaryval != 0)
    {
        remainder = binaryval % 10;
        hexadecimalval = hexadecimalval + remainder * i;
        i = i * 2;
        binaryval = binaryval / 10;
    }

    cout<<hex<<uppercase<<hexadecimalval<<endl;
    out<<hex<<uppercase<<hexadecimalval<<" ";


}




int main()
{
    int val;
    string ins,op,r1,r2,r3,cmnt,binary;
    map<string,string> R_TYPE,I_TYPE,J_TYPE,RegisterValue;
    //Obtaining static values from storage
    R_TYPE = getMap("R_TYPE.txt");
    I_TYPE = getMap("I_TYPE.txt");
    J_TYPE = getMap("J_TYPE.txt");
    RegisterValue = getMap("RegisterValue.txt");
    ifstream inputFile("input.txt");
    ofstream output2;
    output2.open("output_hexa.txt");
    //ifstream inputFile("File/validtestcases.txt");
    //ifstream inputFile("File/invalidtestcases.txt");
    ofstream outputFile("output_bin.txt");
    while(getline(inputFile,ins))
    {
        stringstream ss;
        cout << ins << endl;
        //Checking if instruction is empty or not
        if(ins.size()==0)
        {
            cout << "Invalid: Instruction is empty" << endl;
            outputFile << "Invalid: Instruction is empty" << endl;
            continue;
        }
        ss << ins;
        ss >> op;
        //If operation code is J Type then the program will go in here
        if(J_TYPE.find(op)!=J_TYPE.end())
        {
            val = 0;
            ss >> val;
            //Checking instruction validities
            if(val<0)
            {
                cout << "invalid: value can not be neg" << endl;
                outputFile << "invalid: value can not be neg" << endl;
            }
            else if(val>1023)
            {
                cout << "invalid: value is too large" << endl;
                outputFile << "invalid: value is too large" << endl;
            }
            else if(ss>>cmnt&&(cmnt[0]!='/'||cmnt[1]!='/'))
            {
                cout << "Invalid: comment structure not maintained" << endl;
                outputFile << "Invalid: comment structure not maintained" << endl;
            }
            //Printing machine code
            else
            {
                cout << J_TYPE[op] << " " << decToBin(val,10) << endl;
                outputFile << J_TYPE[op] << " " << decToBin(val,10) << endl;
                binary=J_TYPE[op]+decToBin(val,10);
                BinarytoHex(binary,output2);
            }
            continue;
        }

        //If operation code is R Type then the program will go in here
        if(R_TYPE.find(op)!=R_TYPE.end())
        {
            val = 0;

            ss >> r1;

            if(r1[0]!='$')
            {
                cout << "Invalid: '$' sign is missing before rd" << endl;
                outputFile << "Invalid: '$' sign is missing before rd" << endl;
                continue;
            }
            if(op=="sll"||op=="srl"||op=="din"||op=="dout")
            {
                r2 = "$zero";
            }
            else
            {
                if(r1[r1.size()-1]!=',')
                {
                    cout << "Invalid: ',' is missing after rd" << endl;
                    outputFile << "Invalid: ',' is missing after rd" << endl;
                    continue;
                }
                r1.erase(r1.end()-1);
                ss >> r2;
                if(r2[0]!='$')
                {
                    cout << "Invalid: '$' sign is missing before rs" << endl;
                    outputFile << "Invalid: '$' sign is missing before rs" << endl;
                    continue;
                }

            }
            if(op=="sll"||op=="srl")
            {
                if(r1[r1.size()-1]!=',')
                {
                    cout << "Invalid: ',' is missing after rd" << endl;
                    outputFile << "Invalid: ',' is missing after rd" << endl;
                    continue;
                }
                r1.erase(r1.end()-1);
                val = -1;
                ss >> val;
            }

            //Checking instruction validities
print:
            if(r1=="$zero")
            {
                cout << "Invalid: $zero can not be used" << endl;
                outputFile << "Invalid: $zero can not be used" << endl;
            }
            else if(RegisterValue.find(r1)==RegisterValue.end())
            {
                cout << "Invalid: rd not found" << endl;
                outputFile << "Invalid: rd not found" << endl;
            }
            else if(RegisterValue.find(r2)==RegisterValue.end())
            {
                cout << "Invalid: rs not found" << endl;
                outputFile << "Invalid: rs not found" << endl;
            }
            else if(val<0)
            {
                cout << "Invalid: value can not be neg" << endl;
                outputFile << "Invalid: value can not be neg" << endl;
            }
            else if(val>3)
            {
                cout << "Invalid: value is too large" << endl;
                outputFile << "Invalid: value is too large" << endl;
            }
            else if(ss>>cmnt&&(cmnt[0]!='/'||cmnt[1]!='/'))
            {
                cout << "Invalid: comment structure not maintained" << endl;
                outputFile << "Invalid: comment structure not maintained" << endl;
            }
            //Printing machine code if instruction is valid
            else
            {
                cout << R_TYPE[op] << " " << RegisterValue[r2] << " " << RegisterValue[r1] << " " << decToBin(val,2) << endl;
                outputFile << R_TYPE[op] << " " << RegisterValue[r2] << " " << RegisterValue[r1] << " " << decToBin(val,2) << endl;
                binary=R_TYPE[op]+RegisterValue[r2]+RegisterValue[r1]+decToBin(val,2);
                BinarytoHex(binary,output2);
            }
        }
        //If operation code is I Type then the program will go in here
        else if(I_TYPE.find(op)!=I_TYPE.end())
        {
            val = 0;
            if(op=="lw"||op=="sw")
            {
                ss >> val >> r2;
                if(r2[0]!='(')
                {
                    cout << "Invalid: '(' is missing before rs" << endl;
                    outputFile << "Invalid: '(' is missing before rs" << endl;
                    continue;
                }
                else if(r2[r2.size()-1]!=')')
                {
                    cout << "Invalid: ')' is missing after rs" << endl;
                    outputFile << "Invalid: ')' is missing after rs" << endl;
                    continue;
                }
                else if(r2[1]!='$')
                {
                    cout << "Invalid: '$' sign is missing before rs" << endl;
                    outputFile << "Invalid: '$' sign is missing before rs" << endl;
                    continue;
                }
                r2.erase(r2.begin());
                r2.erase(r2.end()-1);
                //swap(r1,r2);
                if(r1=="$zero")
                {
                    r1 = "";
                    cout << "Invalid: $zero can to be used" << endl;
                    outputFile << "Invalid: $zero can to be used" << endl;
                    continue;
                }
                else if(r2=="$zero")
                {
                    r2 = "";
                    cout << "Invalid: $zero can to be used" << endl;
                    outputFile << "Invalid: $zero can to be used" << endl;
                    continue;
                }
            }
            else
            {
                ss >> r2 >> val;
                if(r2[0]!='$')
                {
                    cout << "Invalid: '$' sign is missing before rs" << endl;
                    outputFile << "Invalid: '$' sign is missing before rs" << endl;
                    continue;
                }
                else if(r2[r2.size()-1]!=',')
                {
                    cout << "Invalid: ',' is missing after rs" << endl;
                    outputFile << "Invalid: ',' is missing after rs" << endl;
                    continue;
                }
                r2.erase(r2.end()-1);
                if(op=="addi"||op=="slti")
                {
                    //swap(r1,r2);
                    if(r1=="$zero")
                    {
                        r1 = "";
                        cout << "Invalid: $zero can to be used" << endl;
                        outputFile << "Invalid: $zero can to be used" << endl;
                        continue;
                    }
                }
            }
            //Checking instruction validities
            if(RegisterValue.find(r1)==RegisterValue.end())
            {
                cout << "Invalid: rd not found" << endl;
                outputFile << "Invalid: rd not found" << endl;
            }
            else if(RegisterValue.find(r2)==RegisterValue.end())
            {
                cout << "Invalid: rs not found" << endl;
                outputFile << "Invalid: rs not found" << endl;
            }
            else if(op!="addi"&&op!="beq"&&op!="bne"&&val<0)
            {
                cout << "Invalid: value can not be neg" << endl;
                outputFile << "Invalid: value can not be neg" << endl;
            }
            else if(val>63)
            {
                cout << "Invalid: value is too large" << endl;
                outputFile << "Invalid: value is too large" << endl;
            }
            else if(val<-63)
            {
                cout << "Invalid: value is too small" << endl;
                outputFile << "Invalid: value is too small" << endl;
            }
            else if(ss>>cmnt&&(cmnt[0]!='/'||cmnt[1]!='/'))
            {
                cout << "Invalid: comment structure not maintained" << endl;
                outputFile << "Invalid: comment structure not maintained" << endl;
            }
            //Printing machine code if instruction is valid
            else
            {
                cout << I_TYPE[op] << " " << RegisterValue[r2] << " " << decToBin(val,6) << endl;
                outputFile << I_TYPE[op] << " " << RegisterValue[r2] << " " << decToBin(val,6) << endl;
                binary=I_TYPE[op]+RegisterValue[r2]+decToBin(val,6);
                BinarytoHex(binary,output2);
            }
        }
        else
        {
            cout << "Invalid: Op code not found" << endl;
            outputFile << "Invalid: Op code not found" << endl;
        }
    }
    inputFile.close();
}

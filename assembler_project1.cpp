#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <bitset>
#include <algorithm>

using namespace std;

string Rformat[] = {"addu", "and", "jr", "nor", "or", "sltu", "sll", "srl", "subu"};
string Lformat[] = {"addiu", "andi", "beq", "bne", "lui", "lw", "ori", "sltiu", "sw", "lb", "sb"};
string jformat[] = {"j", "jal"};
string specialformat = "la";



vector<string> convertInstructionsToBinary(map<int, string>& memory_text,const map<string,int>& memory_label) {
    vector<string> binary_code;
    string binary_instruction;
    for (auto it = memory_text.begin(); it != memory_text.end();++it ) {
        stringstream ss(it->second);
        string first, one, two, three;
        ss >> first >> one >> two >> three;
            if (first == "addiu") {  // i format
                bitset<6> op(9); 
                bitset<5> rs(stoi(two.substr(1))); 
                bitset<5> rt(stoi(one.substr(1))); 
                int immediateValue = 0;
//		cout << "Processing 'addiu' with immediate field: " << three << endl; // 디버깅용 출력
		try {
		    if (three.find("0x") == 0 || three.find("0X") == 0) {
		        // 16진수 값 처리
		        immediateValue = stoi(three, nullptr, 16);
		    } else {
		        // 십진수 값 처리
		        immediateValue = stoi(three);
		    }
		} catch (const std::invalid_argument& e) {
		    std::cerr << "Invalid number format in immediate field: " << three << std::endl;
		    continue; // 혹은 적절한 에러 처리
		}
                bitset<16> immediate(immediateValue);
		binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + immediate.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first =="sltiu"){
		bitset<6> op(0xb);
                bitset<5> rs(stoi(two.substr(1)));
                bitset<5> rt(stoi(one.substr(1)));
                int immediateValue = 0;
                try {
                    if (three.find("0x") == 0 || three.find("0X") == 0) {
                        // 16진수 값 처리
                        immediateValue = stoi(three, nullptr, 16);
                    } else {
                        // 십진수 값 처리
                        immediateValue = stoi(three);
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid number format in immediate field: " << three << std::endl;
                    continue; // 혹은 적절한 에러 처리
                }
                bitset<16> immediate(immediateValue);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + immediate.to_string();
                binary_code.push_back(binary_instruction); 
	    } else if (first == "andi"){
	   	bitset<6> op(0xc);
                bitset<5> rs(stoi(two.substr(1)));
                bitset<5> rt(stoi(one.substr(1)));
                int immediateValue = 0;
//                cout << "Processing 'andi' with immediate field: " << three << endl; // 디버깅용 출력
                try {
                    if (three.find("0x") == 0 || three.find("0X") == 0) {
                        // 16진수 값 처리
                        immediateValue = stoi(three, nullptr, 16);
                    } else {
                        // 십진수 값 처리
                        immediateValue = stoi(three);
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid number format in immediate field: " << three << std::endl;
                    continue; // 혹은 적절한 에러 처리
                }
                bitset<16> immediate(immediateValue);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + immediate.to_string();
                binary_code.push_back(binary_instruction);
            } else if (first == "addu") { // r format
                bitset<6> op(0); 
                bitset<5> rs(stoi(two.substr(1))); 
                bitset<5> rt(stoi(three.substr(1)));
                bitset<5> rd(stoi(one.substr(1)));
                bitset<5> shamt(0);
                bitset<6> funct(0x21);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "and"){
		bitset<6> op(0);
                bitset<5> rs(stoi(two.substr(1)));
                bitset<5> rt(stoi(three.substr(1)));
                bitset<5> rd(stoi(one.substr(1)));
                bitset<5> shamt(0);
                bitset<6> funct(0x24);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	//	cout<<"and debugging"<<binary_instruction <<endl;//and 디버깅
	    } else if (first == "bne") {
                bitset<6> op(5);
                bitset<5> rs(stoi(one.substr(1)));
                bitset<5> rt(stoi(two.substr(1))); 
                auto labelIt = memory_label.find(three);
                if (labelIt != memory_label.end()) {
                    int target = labelIt->second;
                    int PC = it->first; // 현재 명령어의 주소를 PC로 사용합니다.
                    int offset = (target - (PC + 4)) / 4; // 워드 단위 offset 계산

                    bitset<16> immediate(offset); // offset을 16비트 값으로 변환
                    binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + immediate.to_string();
                    binary_code.push_back(binary_instruction);
                } else {
                    cout << "Label not found: " << three << endl;
                }
	    } else if (first == "beq"){
		bitset<6> op(4);
                bitset<5> rs(stoi(one.substr(1)));
                bitset<5> rt(stoi(two.substr(1)));
                auto labelIt = memory_label.find(three);
                if (labelIt != memory_label.end()) {
                    int target = labelIt->second;
                    int PC = it->first; // 현재 명령어의 주소를 PC로 사용합니다.
                    int offset = (target - (PC + 4)) / 4; // 워드 단위 offset 계산

                    bitset<16> immediate(offset); // offset을 16비트 값으로 변환
                    binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + immediate.to_string();
                    binary_code.push_back(binary_instruction);
                } else {
                    cout << "Label not found: " << three << endl;
                }
	    } else if (first == "j") { //j format
                bitset<6> op(2);
                auto labelIt = memory_label.find(one);
                if (labelIt != memory_label.end()) {
                    int target = labelIt->second;
                    bitset<26> immediate(target/4); // offset을 16비트 값으로 변환
                    binary_instruction = op.to_string() + immediate.to_string();
                    binary_code.push_back(binary_instruction);
                } else { 
                    cout << "Label not found: " << three << endl;
                }
	    } else if (first =="jal") {
		bitset<6> op(3);
                auto labelIt = memory_label.find(one);
                if (labelIt != memory_label.end()) {
                    int target = labelIt->second;
                    bitset<26> immediate(target/4); // offset을 16비트 값으로 변환
                    binary_instruction = op.to_string() + immediate.to_string();
                    binary_code.push_back(binary_instruction);
                } else {
                    cout << "Label not found: " << three << endl;
                }
	    } else if (first == "jr") {
		bitset<6> op(0);
                bitset<5> rs(stoi(one.substr(1)));
                bitset<5> rt(0);
                bitset<5> rd(0);
                bitset<5> shamt(0);
                bitset<6> funct(8);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "nor"){
		bitset<6> op(0);
                bitset<5> rs(stoi(two.substr(1)));
                bitset<5> rt(stoi(three.substr(1)));
                bitset<5> rd(stoi(one.substr(1)));
                bitset<5> shamt(0);
                bitset<6> funct(0x27);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "or"){
		bitset<6> op(0);
                bitset<5> rs(stoi(two.substr(1)));
                bitset<5> rt(stoi(three.substr(1)));
                bitset<5> rd(stoi(one.substr(1)));
                bitset<5> shamt(0);
                bitset<6> funct(0x25);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "sltu"){
		bitset<6> op(0);
                bitset<5> rs(stoi(two.substr(1)));
                bitset<5> rt(stoi(three.substr(1)));
                bitset<5> rd(stoi(one.substr(1)));
                bitset<5> shamt(0);
                bitset<6> funct(0x2b);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first =="sll"){
		bitset<6> op(0);
                bitset<5> rs(0);
                bitset<5> rt(stoi(two.substr(1)));
                bitset<5> rd(stoi(one.substr(1)));
                bitset<5> shamt(stoi(three));
                bitset<6> funct(0);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "srl"){
		bitset<6> op(0);
                bitset<5> rs(0);
                bitset<5> rt(stoi(two.substr(1)));
                bitset<5> rd(stoi(one.substr(1)));
                bitset<5> shamt(stoi(three));
                bitset<6> funct(2);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "subu"){
		bitset<6> op(0);
                bitset<5> rs(stoi(two.substr(1)));
                bitset<5> rt(stoi(three.substr(1)));
                bitset<5> rd(stoi(one.substr(1)));
                bitset<5> shamt(0);
                bitset<6> funct(0x23);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + rd.to_string() + shamt.to_string() + funct.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "lui") {
	        bitset<6> op(0xf);
                bitset<5> rs(0);
                bitset<5> rt(stoi(one.substr(1)));
                int immediateValue = 0;
//		cout << "Processing 'lui' with immediate field: " << two << endl; // 디버깅용 출력
                try {
                    if (two.find("0x") == 0 || two.find("0X") == 0) {
                        // 16진수 값 처리
                        immediateValue = stoi(two, nullptr, 16);
                    } else {
                        // 십진수 값 처리
                        immediateValue = stoi(two);
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid number format in immediate field: " << two << std::endl;
                    continue; // 혹은 적절한 에러 처리
                }
                bitset<16> immediate(immediateValue);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + immediate.to_string();
                binary_code.push_back(binary_instruction);
            } else if (first == "ori") {
	        bitset<6> op(0xd);
                bitset<5> rs(stoi(two.substr(1)));
                bitset<5> rt(stoi(one.substr(1)));
                int immediateValue = 0;
//		cout << "Processing 'ori' with immediate field: " << three << endl; // 디버깅용 출력
                try {
                    if (three.find("0x") == 0 || three.find("0X") == 0) {
                        // 16진수 값 처리
                        immediateValue = stoi(three, nullptr, 16);
                    } else {
                        // 십진수 값 처리
                        immediateValue = stoi(three);
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid number format in immediate field: " << three << std::endl;
                    continue; // 혹은 적절한 에러 처리
                }
                bitset<16> immediate(immediateValue);
                binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + immediate.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "lw") { //load
		bitset<6> op(0x23);
	        string rt_str = one;
	        size_t leftParen = two.find("(");
	        size_t rightParen = two.find(")");
	        string offset_str = two.substr(0, leftParen);
	        string rs_str = two.substr(leftParen + 1, rightParen - leftParen - 1);
	        bitset<5> rt(stoi(rt_str.substr(1)));
	        bitset<5> rs(stoi(rs_str.substr(1)));
	        bitset<16> offset(stoi(offset_str));
	        string binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + offset.to_string();
	        binary_code.push_back(binary_instruction);
	    } else if (first == "lb") { //load
                bitset<6> op(0x20);
                string rt_str = one;
                size_t leftParen = two.find("(");
                size_t rightParen = two.find(")");
                string offset_str = two.substr(0, leftParen);
                string rs_str = two.substr(leftParen + 1, rightParen - leftParen - 1);
                bitset<5> rt(stoi(rt_str.substr(1)));
                bitset<5> rs(stoi(rs_str.substr(1)));
                bitset<16> offset(stoi(offset_str));
                string binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + offset.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "lw") { //load
                bitset<6> op(0x23);
                string rt_str = one;
                size_t leftParen = two.find("(");
                size_t rightParen = two.find(")");
                string offset_str = two.substr(0, leftParen);
                string rs_str = two.substr(leftParen + 1, rightParen - leftParen - 1);
                bitset<5> rt(stoi(rt_str.substr(1)));
                bitset<5> rs(stoi(rs_str.substr(1)));
                bitset<16> offset(stoi(offset_str));
                string binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + offset.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "sw") { //load
                bitset<6> op(0x2b);
                string rt_str = one;
                size_t leftParen = two.find("(");
                size_t rightParen = two.find(")");
                string offset_str = two.substr(0, leftParen);
                string rs_str = two.substr(leftParen + 1, rightParen - leftParen - 1);
                bitset<5> rt(stoi(rt_str.substr(1)));
                bitset<5> rs(stoi(rs_str.substr(1)));
                bitset<16> offset(stoi(offset_str));
                string binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + offset.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "sb") { //load
                bitset<6> op(0x28);
                string rt_str = one;
                size_t leftParen = two.find("(");
                size_t rightParen = two.find(")");
                string offset_str = two.substr(0, leftParen);
                string rs_str = two.substr(leftParen + 1, rightParen - leftParen - 1);
                bitset<5> rt(stoi(rt_str.substr(1)));
                bitset<5> rs(stoi(rs_str.substr(1)));
                bitset<16> offset(stoi(offset_str));
                string binary_instruction = op.to_string() + rs.to_string() + rt.to_string() + offset.to_string();
                binary_code.push_back(binary_instruction);
	    } else if (first == "la") { //special case
                auto labelIt = memory_label.find(two);
                if (labelIt != memory_label.end()) {
                    int addr = labelIt->second;
		    // Address를 16비트로 나눕니다.
                    int addr1 = (addr >> 16) & 0xFFFF; // 상위 16비트
                    int addr2 = addr & 0xFFFF; // 하위 16비트
		    
                    // 16진수 문자열로 변환
	            stringstream ss_addr1, ss_addr2;
	            ss_addr1 << "0x" << hex << addr1;
	            ss_addr2 << "0x" << hex << addr2;
                    // 명령어 생성
	            string lui_instruction = "lui " + one + ", " + ss_addr1.str();
		    memory_text[it->first +4]= lui_instruction;	            
 	            if (addr2 != 0) {
	           	 string ori_instruction = "ori " + one + ", " + one + ", " + ss_addr2.str();
  		         memory_text[it->first +8] = ori_instruction;
		    }
   		    } else {
     		   cout << "Label not found: " << two << endl;
  		    }
		}	 
	    }    
    return binary_code;
}


map<string, int> memory_label;
map<int, int> memory_data;
map<int, string> memory_text;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file.s>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    string line;
    bool dataSection = false, textSection = false;
    int dataAddress = 0x10000000, textAddress = 0x00400000;

    while (getline(inputFile, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        if (line.empty()) continue; // 공백 줄은 무시합니다.

        stringstream ss(line);
        string word;
        ss >> word;

        if (word == ".data") {
            dataSection = true;
            textSection = false;
            continue;
        } else if (word == ".text") {
            textSection = true;
            dataSection = false;
            continue;
        }

        if (dataSection) {
            stringstream dataStream(line); // 데이터 섹션 처리를 위한 새로운 stringstream
            dataStream >> word; // 첫 번째 단어를 다시 읽음
            if (word.back() == ':') { // Label 처리
                word.pop_back(); // 콜론 제거
                memory_label[word] = dataAddress;
                
                if (dataStream >> word) { // 라벨 처리 후 남은 줄에서 다음 단어를 읽음
                    if (word == ".word") {
                        int value;
                        while (dataStream >> hex >> value) {
                            memory_data[dataAddress] = value;
                            dataAddress += 4;
		    	}
                    }
                }
            } else if (word == ".word") {
                unsigned int value;
                while (dataStream >> hex >> value) {
                    memory_data[dataAddress] = value;
                    dataAddress += 4;
                }
            }
        } else if (textSection) {
            if (word.back() == ':') { // Label 처리
                word.pop_back(); // 콜론 제거
                memory_label[word] = textAddress;
        	string restOfLine;
        	getline(ss, restOfLine); // 라벨 다음의 문자열을 가져옴
        	size_t firstNonSpace = restOfLine.find_first_not_of(" \t"); // 공백이 아닌 첫 문자 위치 찾기
        	if (firstNonSpace != string::npos) {
            	restOfLine = restOfLine.substr(firstNonSpace); // 공백 제거
            	memory_text[textAddress] = restOfLine; // 공백이 제거된 명령어를 memory_text에 추가
            	textAddress += 4; // 명령어 주소 증가
        	}
            } else {
                // 'la' 명령어 처리
                if (word == "la") {
                    string first, one, two;
		    ss. clear();
		    ss.str(line);
		    ss>> first >> one >>two;
		  //  cout<<"word/line:"<<word<<"/"<<line<<endl;
		  //  cout<<"first/one/two:"<<first<<"/"<<one<<"/"<<two<<endl;
		    auto labelIt = memory_label.find(two);
                    if (labelIt != memory_label.end()) {
                    	int addr = labelIt->second;
                    // Address를 16비트로 나눕니다.
                    	int addr1 = (addr >> 16) & 0xFFFF; // 상위 16비트
                    	int addr2 = addr & 0xFFFF; // 하위 16비트
                   // 16진수 문자열로 변환
                    	stringstream ss_addr1, ss_addr2;
                   	ss_addr1 << "0x" << hex << addr1;
                    	ss_addr2 << "0x" << hex << addr2;
                    // 명령어 생성
                   	string lui_instruction = "lui " + one + ", " + ss_addr1.str();
		  	string ori_instruction = "ori " + one + ", " + one + ", " + ss_addr2.str(); 
		 //   	memory_text[textAddress]=lui_instruction;
		//    cout<<"lui_instruction:"<<lui_instruction<<endl;
		//    cout<<"ori_instruction:"<<ori_instruction<<endl;
  		    memory_text[textAddress]=lui_instruction;
                    textAddress+=4;
                    if (addr2 != 0){
                    memory_text[textAddress]=ori_instruction;
                    textAddress+=4;
                    }
		    }
		} else {
                    memory_text[textAddress] = line; // 수정된 부분
                    textAddress += 4; // 그 외 명령어는 4만큼 증가
                }
            }
	}
    }
    inputFile.close();
    vector<string> binary_code = convertInstructionsToBinary(memory_text,memory_label);
    // output 파일명 생성
    string inputFilename(argv[1]);
    size_t dotPos = inputFilename.find_last_of(".");
    string outputFilename = inputFilename.substr(0, dotPos) + ".o";

    // output 파일 오픈
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return 1;
    }

    // Text section size 출력
    outputFile << "0x" << hex << memory_text.size() * 4 << endl;
    // Data section size 출력
    outputFile << "0x" << hex << memory_data.size() * 4 << endl;

    // Instructions 출력
    for (const auto& instruction : binary_code) {
        unsigned long instructionValue = bitset<32>(instruction).to_ulong();
        outputFile << "0x" << hex << instructionValue << endl;
    }

    // Values 출력
    for (const auto& [key, value] : memory_data) {
        outputFile << "0x" << hex << value << endl;
    }

    outputFile.close();
    // 변환된 바이너리 코드 출력
//    cout << "Binary Code:" << endl;
//    for (const string& binary : binary_code) {
//        cout << binary << endl;
//    }
    // Debug: Print all memory maps
//    cout << "Memory Label:" << endl;
//    for (auto& label : memory_label) {
//        cout << label.first << " : " << hex << label.second << endl;
//    }

//    cout << "\nMemory Data:" << endl;
//    for (auto& data : memory_data) {
//        cout << hex << data.first << " : " << data.second << endl;
//    }

//    cout << "\nMemory Text:" << endl;
//    for (auto& text : memory_text) {
//        cout << hex << text.first << " : " << text.second << endl;
//    }    
    return 0;
}

//
//  main.cpp
//  데구_1차플젝
//
//  Created by 최우진 on 9/16/24. int argc, const char * argv[]
//
#include <iostream>
#include <fstream>
#include <string>

int QUEUE_SIZE = 100;  // 큐의 초기 크기

// 자막 정보를 저장하는 큐의 노드 클래스
class SubtitleQueueNode {
public:
    std::string time;  // 자막 시간
    std::string content;  // 자막 내용
    SubtitleQueueNode* next;  // 다음 노드를 가리키는 포인터

    // 생성자
    SubtitleQueueNode(const std::string& t, const std::string& c) {
        time = t;          // 전달받은 time 값을 멤버 변수에 할당
        content = c;       // 전달받은 content 값을 멤버 변수에 할당
        next = nullptr;    // next 포인터를 nullptr로 초기화
    }
};

// 자막 정보를 저장하는 큐 클래스
class SubtitleQueue {
private:
    SubtitleQueueNode* front;  // 큐의 첫 번째 노드
    SubtitleQueueNode* rear;  // 큐의 마지막 노드
    int count;  // 현재 큐에 저장된 노드 개수

public:
    // 생성자
    SubtitleQueue() : front(nullptr), rear(nullptr), count(0) {}

    // 큐에 자막을 추가하는 함수
    bool push(const std::string& time, const std::string& content) {
        if (count >= QUEUE_SIZE) {  // 큐가 가득 찼을 때
            QUEUE_SIZE *= 2;  // 큐의 크기를 두 배로 확장
            std::cout << "Queue size doubled" << std::endl;
        }
        SubtitleQueueNode* newNode = new SubtitleQueueNode(time, content);  // 새로운 노드 생성
        if (rear) {
            rear->next = newNode;  // 마지막 노드 다음에 새 노드를 추가
        } else {
            front = newNode;  // 큐가 비어 있을 때 첫 번째 노드를 추가
        }
        rear = newNode;  // 새 노드를 마지막 노드로 설정
        count++;  // 노드 개수를 증가
        return true;
    }

    // 큐에서 자막을 꺼내는 함수
    SubtitleQueueNode* pop() {
        if (isEmpty()) {  // 큐가 비어 있을 때
            std::cout << "===== ERROR =====\n200\n===============\n";
            return nullptr;
        }
        SubtitleQueueNode* temp = front;  // 첫 번째 노드를 임시로 저장
        front = front->next;  // 첫 번째 노드를 제거하고 다음 노드를 첫 번째로 설정
        if (!front) rear = nullptr;  // 큐가 비어 있으면 마지막 노드를 nullptr로 설정
        count--;  // 노드 개수를 감소
        return temp;
    }

    // 큐가 비어 있는지 확인하는 함수
    bool isEmpty() const {
        return count == 0; //큐 안에 객체가 0개 이면 true 아니면 false
    }
};

// 자막 정보를 저장하는 BST 노드 클래스
class SubtitleBSTNode {
public:
    std::string time;  // 자막 시간
    std::string content;  // 자막 내용
    SubtitleBSTNode* left;  // 왼쪽 자식 노드
    SubtitleBSTNode* right;  // 오른쪽 자식 노드

    //생성자
    SubtitleBSTNode(const std::string& t, const std::string& c) {
        time = t;           // 전달받은 time 값을 멤버 변수 time에 할당
        content = c;        // 전달받은 content 값을 멤버 변수 content에 할당
        left = nullptr;     // left 포인터를 nullptr로 초기화하여 왼쪽 자식이 없음을 나타냄
        right = nullptr;    // right 포인터를 nullptr로 초기화하여 오른쪽 자식이 없음을 나타냄
    }
};

// 자막 정보를 저장하는 BST 클래스
class SubtitleBST {
private:
    SubtitleBSTNode* root;  // 루트 노드

    // BST에 새로운 노드를 추가하는 재귀 함수
    SubtitleBSTNode* insert(SubtitleBSTNode* node, const std::string& time, const std::string& content) {
        if (!node) return new SubtitleBSTNode(time, content);  // 새로운 노드를 생성하여 반환
        if (time < node->time) node->left = insert(node->left, time, content);  // 자막 시간이 현재 노드보다 이전이면 왼쪽에 추가
        else node->right = insert(node->right, time, content);  // 자막 시간이 현재 노드보다 같거나 이후면 오른쪽에 추가
        return node;
    }

    // 중위 순회로 BST를 출력하는 재귀 함수
    void inorderPrint(SubtitleBSTNode* node) const {
        if (!node) return;
        inorderPrint(node->left);  // 왼쪽 서브 트리 방문
        std::cout << node->time << " - " << node->content << std::endl;  // 현재 노드 출력
        inorderPrint(node->right);  // 오른쪽 서브 트리 방문
    }

public:
    // 생성자
    SubtitleBST() : root(nullptr) {}

    // BST에 노드를 삽입하는 함수
    void insert(const std::string& time, const std::string& content) {
        root = insert(root, time, content);  // 루트 노드를 기준으로 삽입
    }

    // BST의 내용을 중위 순서로 출력하는 함수
    void print() const {
        if (!root) {  // BST가 비어 있을 때
            std::cout << "===== ERROR =====\n300\n===============\n";
            return;
        }
        std::cout << "===== PRINT =====\n";
        inorderPrint(root);  // 중위 순서로 출력
        std::cout << "===============\n";
    }
};

// 자막 리스트의 내용 노드 클래스
class SubtitleListNode {
public:
    std::string time;  // 자막 시간
    std::string content;  // 자막 내용
    SubtitleListNode* next;  // 다음 자막 노드를 가리키는 포인터

    SubtitleListNode(const std::string& t, const std::string& c)
        : time(t), content(c), next(nullptr) {}
};

// 섹션 리스트의 헤더 노드 클래스
class SectionListNode {
public:
    int sectionNum;  // 섹션 번호
    SubtitleListNode* subtitleHead;  // 자막 리스트의 첫 번째 노드
    SectionListNode* next;  // 다음 섹션 노드를 가리키는 포인터

    SectionListNode(int num) : sectionNum(num), subtitleHead(nullptr), next(nullptr) {}
};

// 섹션 정보를 저장하는 리스트 클래스 (2차원 연결 리스트)
class SectionList {
private:
    SectionListNode* head;  // 섹션 리스트의 첫 번째 헤더 노드

public:
    SectionList() : head(nullptr) {}

    // 섹션을 추가하는 함수
    void addSection(int sectionNum, const std::string& time, const std::string& content) {
        SectionListNode* sectionNode = findSection(sectionNum);
        if (!sectionNode) {
            sectionNode = new SectionListNode(sectionNum);
            sectionNode->next = head;
            head = sectionNode;
        }
        SubtitleListNode* newSubtitle = new SubtitleListNode(time, content);
        newSubtitle->next = sectionNode->subtitleHead;
        sectionNode->subtitleHead = newSubtitle;
    }

    // 섹션을 찾는 함수
    SectionListNode* findSection(int sectionNum) const {
        SectionListNode* current = head;
        while (current) {
            if (current->sectionNum == sectionNum) return current;
            current = current->next;
        }
        return nullptr;
    }

    // 특정 섹션 번호의 자막을 출력하는 함수
    void printSection(int sectionNum) const {
        SectionListNode* section = findSection(sectionNum);
        if (!section) {
            std::cout << "===== ERROR =====\n400\n===============\n";
            return;
        }
        SubtitleListNode* current = section->subtitleHead;
        std::cout << "===== PRINT SECTION " << sectionNum << " =====\n";
        while (current) {
            std::cout << current->time << " - " << current->content << std::endl;
            current = current->next;
        }
        std::cout << "===============\n";
    }
};

// 프로그램 전체를 관리하는 매니저 클래스
class Manager {
private:
    SubtitleQueue queue;  // 자막 큐 객체
    SubtitleBST bst;  // 자막 BST 객체
    SectionList sectionList;  // 섹션 리스트 객체

public:
    // LOAD 명령을 처리하는 함수
    void loadSubtitles(const std::string& filename) {
        std::ifstream file(filename);  // 파일을 열기
        if (!file.is_open()) {  // 파일을 열 수 없는 경우
            std::cout << "===== ERROR =====\n100\n===============\n";
            return;
        }
        std::string line;
        while (getline(file, line)) {  // 파일의 각 줄을 읽음
            int pos = line.find(' ');  // 첫 번째 공백의 위치를 찾음
            if (pos == -1) continue;  // 공백이 없으면 다음 줄로 이동 (find가 실패하면 -1 반환)
            
            std::string time = line.substr(0, pos);  // 자막 시간 부분 추출
            std::string content = line.substr(pos + 1);  // 자막 내용 부분 추출
            queue.push(time, content);  // 큐에 자막을 추가
        }
        file.close();
        std::cout << "===== LOAD =====\nLoad Complete\n===============\n";
    }

    // QPOP 명령을 처리하는 함수
    void qpop() {
        if (queue.isEmpty()) {  // 큐가 비어 있을 때
            std::cout << "===== ERROR =====\n200\n===============\n";
            return;
        }
        while (!queue.isEmpty()) {  // 큐가 비어질 때까지
            SubtitleQueueNode* node = queue.pop();  // 큐에서 노드를 꺼냄
            bst.insert(node->time, node->content);  // BST에 노드를 삽입
            delete node;  // 노드 메모리 해제
        }
        std::cout << "===== QPOP =====\nSuccess\n===============\n";
    }

    // SECTION 명령어로 섹션을 추가하는 함수
    void section(int sectionNum, const std::string& startTime, const std::string& endTime) {
        // BST에서 startTime과 endTime 사이의 자막을 찾아서 섹션에 추가
        // 이 부분은 사용자에게 입력된 시간 구간에 따라 자막을 가져오는 기능을 구현해야 합니다.
        sectionList.addSection(sectionNum, startTime, endTime);  // 예시로 단순 추가
    }

    // 특정 섹션 번호의 자막을 출력하는 함수
    void printSection(int sectionNum) {
        sectionList.printSection(sectionNum);
    }
    
    void printBST() {
            bst.print();  // BST의 내용을 출력
        }

    // command.txt 파일을 읽고 명령어를 처리하는 함수
    void executeCommands(const std::string& commandFile) {
        std::ifstream file(commandFile);  // command.txt 파일을 열기
        if (!file.is_open()) {
            std::cout << "===== ERROR =====\n1000\n===============\n";
            return;
        }
        std::string command;
        while (file >> command) {  // 파일에서 명령어를 읽음
            if (command == "LOAD") {
                loadSubtitles("subtitle.txt");  // LOAD 명령어 실행
            } else if (command == "QPOP") {
                qpop();  // QPOP 명령어 실행
            } else if (command == "SECTION") {
                int sectionNum;
                std::string startTime, endTime;
                file >> sectionNum >> startTime >> endTime;
                section(sectionNum, startTime, endTime);  // SECTION 명령어 실행
            } else if (command == "PRINT") {
                if (file.peek() == '\n' || file.eof()) {  // 인자가 없으면 BST 출력
                    printBST();
                } else {  // 인자가 있으면 섹션 번호로 해석
                    int sectionNum;
                    file >> sectionNum;
                    printSection(sectionNum);
                }
            }
 else {
                std::cout << "===== ERROR =====\n1000\n===============\n";  // 잘못된 명령어 처리
            }
        }
        file.close();
    }
};

int main(int argc, const char * argv[]) {
    Manager manager;  // 매니저 객체 생성
    manager.executeCommands("command.txt");  // command.txt 파일에서 명령어 실행
    return 0;  // 프로그램 종료
}

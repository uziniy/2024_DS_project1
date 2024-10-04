//
//  main.cpp
//  대구_1차플젝
//
//  Created by 최우진 on 9/16/24. int argc, const char * argv[]
//
#include <iostream>  // 입출력 스트림 라이브러리 포함
#include <fstream>  // 파일 입출력 라이브러리 포함
#include <string>  // 문자열 사용을 위한 라이브러리 포함

int QUEUE_SIZE = 100;  // 큐의 초기 크기를 100으로 설정

// 자막 정보를 저장하는 큐의 노드 클래스
class SubtitleQueueNode {
public:
    std::string time;  // 자막 시간을 저장하는 변수
    std::string content;  // 자막 내용을 저장하는 변수
    SubtitleQueueNode* next;  // 다음 노드를 가리키는 포인터

    // 생성자
    SubtitleQueueNode(const std::string& t, const std::string& c) {
        time = t;  // 전달받은 시간(time)을 멤버 변수에 저장
        content = c;  // 전달받은 내용(content)을 멤버 변수에 저장
        next = nullptr;  // 다음 노드를 가리키는 포인터를 nullptr로 초기화
    }
};

// 자막 정보를 저장하는 큐 클래스
class SubtitleQueue {
private:
    SubtitleQueueNode* front;  // 큐의 첫 번째 노드를 가리키는 포인터
    SubtitleQueueNode* rear;  // 큐의 마지막 노드를 가리키는 포인터
    int count;  // 현재 큐에 저장된 노드의 개수

public:
    // 생성자
    SubtitleQueue() : front(nullptr), rear(nullptr), count(0) {}  // 큐의 초기 상태를 설정

    // 큐에 자막을 추가하는 함수
    bool push(const std::string& time, const std::string& content) {
        if (count >= QUEUE_SIZE) {  // 큐가 가득 찬 경우
            QUEUE_SIZE *= 2;  // 큐의 크기를 두 배로 확장
            std::cout << "Queue size doubled" << std::endl;  // 큐 크기 변경 메시지 출력
        }
        SubtitleQueueNode* newNode = new SubtitleQueueNode(time, content);  // 새로운 노드 생성
        if (rear) {
            rear->next = newNode;  // 마지막 노드 다음에 새 노드를 추가
        }
        else {
            front = newNode;  // 큐가 비어 있을 때 첫 번째 노드를 추가
        }
        rear = newNode;  // 마지막 노드를 새로 생성한 노드로 변경
        count++;  // 큐의 노드 개수 증가
        return true;
    }

    // 큐에서 자막을 꺼내는 함수
    SubtitleQueueNode* pop() {
        if (isEmpty()) {  // 큐가 비어있는지 확인
            std::cout << "===== ERROR =====\n200\n===============\n";  // 에러 메시지 출력
            return nullptr;
        }
        SubtitleQueueNode* temp = front;  // 첫 번째 노드를 임시 저장
        front = front->next;  // 첫 번째 노드를 제거하고 다음 노드를 첫 번째로 설정
        if (!front) rear = nullptr;  // 큐가 비어 있으면 마지막 노드를 nullptr로 설정
        count--;  // 노드 개수 감소
        return temp;
    }

    // 큐가 비어있는지 확인하는 함수
    bool isEmpty() const {
        return count == 0;  // 큐 안에 객체가 0개이면 true 아니면 false 반환
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
    SectionList() : head(nullptr) {}  // 섹션 리스트의 헤더를 nullptr로 초기화

    // 섹션을 추가하는 함수
    void addSection(int sectionNum, const std::string& time, const std::string& content) {
        SectionListNode* sectionNode = findSection(sectionNum);  // 섹션 찾기
        if (!sectionNode) {  // 섹션이 없으면 새로 생성
            sectionNode = new SectionListNode(sectionNum);
            sectionNode->next = head;
            head = sectionNode;
        }
        SubtitleListNode* newSubtitle = new SubtitleListNode(time, content);  // 새 자막 노드 생성
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
            std::cout << "===== ERROR =====\n400\n===============\n";  // 에러 메시지 출력
            return;
        }
        SubtitleListNode* current = section->subtitleHead;
        std::cout << "===== PRINT SECTION " << sectionNum << " =====\n";
        while (current) {
            std::cout << current->time << " - " << current->content << std::endl;  // 자막 시간과 내용 출력
            current = current->next;
        }
        std::cout << "===============\n";
    }
};

// 자막 정보를 저장하는 BST 노드 클래스
class SubtitleBSTNode {
public:
    std::string time;  // 자막 시간
    std::string content;  // 자막 내용
    SubtitleBSTNode* left;  // 왼쪽 자식 노드
    SubtitleBSTNode* right;  // 오른쪽 자식 노드

    // 생성자
    SubtitleBSTNode(const std::string& t, const std::string& c) {
        time = t;  // 전달받은 시간(time)을 멤버 변수에 저장
        content = c;  // 전달받은 내용(content)을 멤버 변수에 저장
        left = nullptr;  // 왼쪽 자식을 nullptr로 초기화
        right = nullptr;  // 오른쪽 자식을 nullptr로 초기화
    }
};

// 자막 정보를 저장하는 BST 클래스
class SubtitleBST {
private:
    SubtitleBSTNode* root;  // 루트 노드

    // BST에 새로운 노드를 추가하는 재귀 함수
    SubtitleBSTNode* insert(SubtitleBSTNode* node, const std::string& time, const std::string& content) {
        if (!node) return new SubtitleBSTNode(time, content);  // 새로운 노드 생성
        if (time < node->time) node->left = insert(node->left, time, content);  // 왼쪽에 노드 추가
        else node->right = insert(node->right, time, content);  // 오른쪽에 노드 추가
        return node;
    }

    // 특정 노드를 삭제하는 재귀 함수
    SubtitleBSTNode* deleteNode(SubtitleBSTNode* node, const std::string& time) {
        if (!node) return nullptr;

        if (time < node->time) node->left = deleteNode(node->left, time);
        else if (time > node->time) node->right = deleteNode(node->right, time);
        else {
            if (!node->left) {
                SubtitleBSTNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                SubtitleBSTNode* temp = node->left;
                delete node;
                return temp;
            }
            else {
                SubtitleBSTNode* temp = findMin(node->right);
                node->time = temp->time;
                node->content = temp->content;
                node->right = deleteNode(node->right, temp->time);
            }
        }
        return node;
    }

    // 최소값을 찾는 함수
    SubtitleBSTNode* findMin(SubtitleBSTNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    // 중위 순회로 BST를 출력하는 재귀 함수
    void inorderPrint(SubtitleBSTNode* node) const {
        if (!node) return;
        inorderPrint(node->left);  // 왼쪽 서브 트리 방문
        std::cout << node->time << " - " << node->content << std::endl;  // 현재 노드 출력
        inorderPrint(node->right);  // 오른쪽 서브 트리 방문
    }

    // 주어진 시간 구간 내의 자막을 찾아 SectionList에 추가하는 재귀 함수
    void findInRange(SubtitleBSTNode* node, const std::string& startTime, const std::string& endTime, SectionList& sectionList, int sectionNum) const {
        if (!node) return;

        // startTime보다 큰 시간일 때 왼쪽 트리 방문
        if (startTime < node->time)
            findInRange(node->left, startTime, endTime, sectionList, sectionNum);

        // 노드의 시간이 startTime과 endTime 사이일 때 자막을 섹션에 추가
        if (startTime <= node->time && node->time <= endTime)
            sectionList.addSection(sectionNum, node->time, node->content);

        // endTime보다 작은 시간일 때 오른쪽 트리 방문
        if (endTime > node->time)
            findInRange(node->right, startTime, endTime, sectionList, sectionNum);
    }

public:
    // 생성자
    SubtitleBST() : root(nullptr) {}

    // BST에 노드를 삽입하는 함수
    void insert(const std::string& time, const std::string& content) {
        root = insert(root, time, content);  // 루트 노드부터 시작하여 삽입
    }

    // BST의 내용을 중위 순서로 출력하는 함수
    void print() const {
        if (!root) {  // BST가 비어있을 때
            std::cout << "===== ERROR =====\n300\n===============\n";  // 에러 메시지 출력
            return;
        }
        std::cout << "===== PRINT =====\n";
        inorderPrint(root);  // 중위 순회로 출력
        std::cout << "===============\n";
    }

    // 특정 시간과 동일한 자막 삭제
    void deleteEqualTime(const std::string& time) {
        if (!deleteNode(root, time)) {
            std::cout << "===== ERROR =====\n3000\n===============\n";  // 에러 코드 출력
        }
    }

    // 특정 시간보다 이전 자막들 삭제
    void deleteUnderTime(const std::string& time) {
        if (!root) {
            std::cout << "===== ERROR =====\n3000\n===============\n";
            return;
        }
        deleteUnder(root, time);
    }

    // 시간 구간 내 자막을 SectionList에 추가하는 함수
    void getSubtitlesInRange(const std::string& startTime, const std::string& endTime, SectionList& sectionList, int sectionNum) const {
        findInRange(root, startTime, endTime, sectionList, sectionNum);
    }


    void deleteUnder(SubtitleBSTNode*& node, const std::string& time) {
        if (!node) return;

        // 왼쪽 서브트리부터 처리
        deleteUnder(node->left, time);

        // 현재 노드가 삭제 조건에 맞으면 출력 후 삭제
        if (node->time < time) {
            std::cout << "Deleting: " << node->time << std::endl;
            SubtitleBSTNode* temp = node;
            node = node->right;
            delete temp;

            // 현재 노드를 갱신한 상태로 다시 deleteUnder 호출
            deleteUnder(node, time);
        }
    }

};

// 프로그램 전체를 관리하는 매니저 클래스
class Manager {
private:
    SubtitleQueue queue;  // 자막 큐 객체 생성
    SubtitleBST bst;  // 자막 BST 객체 생성
    SectionList sectionList;  // 섹션 리스트 객체 생성

public:
    // LOAD 명령을 처리하는 함수
    void loadSubtitles(const std::string& filename) {
        std::ifstream file(filename);  // 파일을 열기
        if (!file.is_open()) {  // 파일이 열리지 않는 경우
            std::cout << "===== ERROR =====\n100\n===============\n";
            return;
        }
        std::cout << "===== LOAD =====\n";
        std::string line;
        while (getline(file, line)) {  // 파일에서 각 줄을 읽음
            int pos = line.find(' ');  // 첫 번째 공백 위치 찾기
            if (pos == -1) continue;  // 공백이 없으면 다음 줄로 넘어감

            std::string time = line.substr(0, pos);  // 자막 시간 부분 추출
            std::string content = line.substr(pos + 1);  // 자막 내용 부분 추출
            queue.push(time, content);  // 큐에 자막 추가
            std::cout << time << " - " << content << std::endl;
        }
        file.close();  // 파일 닫기
        std::cout << "=====Load Complete=====\n";
    }

    // QPOP 명령을 처리하는 함수
    void qpop() {
        if (queue.isEmpty()) {  // 큐가 비어 있는지 확인
            std::cout << "===== ERROR =====\n200\n===============\n";
            return;
        }
        while (!queue.isEmpty()) {  // 큐가 비어질 때까지
            SubtitleQueueNode* node = queue.pop();  // 큐에서 노드 꺼내기
            bst.insert(node->time, node->content);  // BST에 노드 삽입
            delete node;  // 노드 메모리 해제
        }
        std::cout << "===== QPOP =====\nSuccess\n===============\n";
    }

    // SECTION 명령어로 섹션을 추가하는 함수
    void section(int sectionNum, const std::string& startTime, const std::string& endTime) {
        bst.getSubtitlesInRange(startTime, endTime, sectionList, sectionNum);  // BST에서 시간 구간 내 자막을 SectionList에 추가
    }

    // 특정 섹션 번호의 자막을 출력하는 함수
    void printSection(int sectionNum) {
        sectionList.printSection(sectionNum);  // 섹션 리스트에서 섹션 출력
    }

    // BST의 자막 내용을 출력하는 함수
    void printBST() {
        bst.print();  // BST 출력
    }

    // command.txt 파일을 읽고 명령어를 처리하는 함수
    void executeCommands(const std::string& commandFile) {
        std::ifstream file(commandFile);  // command.txt 파일 열기
        if (!file.is_open()) {
            std::cout << "===== ERROR =====\n1000\n===============\n";  // 파일 열기 오류 시 에러 메시지 출력
            return;
        }
        std::string command;
        while (file >> command) {  // 파일에서 명령어 읽기
            if (command == "LOAD") {
                loadSubtitles("subtitle.txt");  // LOAD 명령어 실행
            }
            else if (command == "QPOP") {
                qpop();  // QPOP 명령어 실행
            }
            else if (command == "SECTION") {
                int sectionNum;
                std::string startTime, endTime;
                file >> sectionNum >> startTime >> endTime;
                section(sectionNum, startTime, endTime);  // SECTION 명령어 실행
            }
            else if (command == "PRINT") {
                if (file.peek() == '\n' || file.eof()) {  // 인자가 없으면 BST 출력
                    printBST();
                }
                else {  // 인자가 있으면 섹션 번호로 해석
                    int sectionNum;
                    file >> sectionNum;
                    printSection(sectionNum);
                }
            }
            else if (command == "DELETE") {
                std::string condition, time;
                file >> condition >> time;
                if (condition == "EQUAL") {
                    bst.deleteEqualTime(time);  // 특정 시간과 동일한 자막 삭제
                    std::cout << "DELETE EQUAL - ";
                    std::cout << time<<std::endl;
                }
                else if (condition == "UNDER") {
                    bst.deleteUnderTime(time);  // 특정 시간보다 이전 자막들 삭제
                    std::cout << "DELETE UNDER - ";
                    std::cout << time << std::endl;
                }
                else {
                    std::cout << "===== ERROR =====\n1000\n===============\n";
                }
            }
            else {
                std::cout << "===== ERROR =====\n1000\n===============\n";  // 잘못된 명령어 처리
            }
        }
        file.close();  // 파일 닫기
    }
};

int main(int argc, const char* argv[]) {
    Manager manager;  // 매니저 객체 생성
    manager.executeCommands("command.txt");  // command.txt 파일에서 명령어 실행
    return 0;  // 프로그램 종료
}

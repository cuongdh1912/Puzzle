//
//  main.cpp
//  PuzzleGame
//
//  Created by Cuong Do Hung on 4/10/20.
//  Copyright © 2020 Cuong Do Hung. All rights reserved.
//

#include <iostream>
#include <vector>
const int puzzleSize = 3;
const int blankSquare = 0;
int goalState[puzzleSize][puzzleSize] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
int initialState[puzzleSize][puzzleSize] = {{2, 3, 5}, {1, 0, 8}, {4, 7, 6}};

enum Actions {
    Left, Right, Up, Down
};

class Point {
public: int i, j;
    Point(int i, int j) {
        this->i = i;
        this->j = j;
    }
    bool operator!=(const Point& point) {
        return i != point.i || j != point.j;
    }
    bool operator==(const Point& lhs)
    {
        return lhs.i == i && lhs.j == j;
    }
};

class State {
    Point blankPoint = Point(1, 1);
    int state[puzzleSize][puzzleSize];
    int pathCost[puzzleSize * puzzleSize];
public:
    State(int s[puzzleSize][puzzleSize]) {
        for (int i = 0; i < puzzleSize; i++) {
            for (int j = 0; j < puzzleSize; j++) {
                state[i][j] = s[i][j];
                if (state[i][j] == 0) {
                    blankPoint.i = i;
                    blankPoint.j = j;
                }
                // calcuate pathCost
                for (int k = 0; k < puzzleSize; k++) {
                    for (int l = 0; l < puzzleSize; l++) {
                        if (goalState[k][l] == initialState[i][j]) {
                            int cost = abs(i - k) + abs(j - l);
                            pathCost[initialState[i][j]] = cost;
                        }
                    }
                }
            }
        }
    }
    State(){
        
    }
    // clone self
    State clone() {
        State newState = State();
        for (int i = 0; i < puzzleSize; i++) {
            for (int j = 0; j < puzzleSize; j++) {
                newState.state[i][j] = state[i][j];
                if (state[i][j] == 0) {
                    blankPoint.i = i;
                    blankPoint.j = j;
                }
                newState.pathCost[state[i][j]] = pathCost[state[i][j]];
            }
        }
        newState.blankPoint = blankPoint;
        return newState;
    }
    Point calculateNewPoint(Actions action) {
        Point newPoint = Point(blankPoint.i, blankPoint.j);
        switch (action) {
            case Left:
                if (blankPoint.j > 0) {
                    newPoint.j -= 1;
                }
                break;
            case Right:
                if (blankPoint.j < puzzleSize - 1) {
                    newPoint.j += 1;
                }
                break;
            case Up:
                if (blankPoint.i > 0){
                    newPoint.i -= 1;
                }
                break;
            case Down:
                if (blankPoint.i < puzzleSize - 1){
                    newPoint.i += 1;
                }
                break;
            default:
                break;
        }
        return newPoint;
    }
public:
    int calculateAfterMove(Point point) {
        int oldValue = pathCost[state[point.i][point.j]];
        if (state[point.i][point.i] == goalState[point.i][point.j]) {
            pathCost[state[point.i][point.j]] = 0;
            return oldValue;
        }else { // if tile is not the same
            for (int k = 0; k < puzzleSize; k++) {
                for (int l = 0; l < puzzleSize; l++) {
                    if (goalState[k][l] == state[point.i][point.j]) {
                        int newPathCost = abs(point.i - k) + abs(point.j - l);
                        if (oldValue - newPathCost > 0) {
                            pathCost[state[point.i][point.j]] = newPathCost;
                        }
                        return oldValue - newPathCost;
                    }
                }
            }
        }
        return -1;
    }
    int doAction(Actions action) {
        Point newPoint = calculateNewPoint(action);
        if (newPoint != blankPoint) {
            state[blankPoint.i][blankPoint.j] = state[newPoint.i][newPoint.j];
            state[newPoint.i][newPoint.j] = blankSquare;
            int c = calculateAfterMove(blankPoint);
            blankPoint = newPoint;
            return c;
        }
        return 0;
    }
    int totalPathCost() {
        int total = 0;
        for (int i = 1 ;i < puzzleSize * puzzleSize; i++) {
            total += pathCost[i];
        }
        return total;
    }
    
    void printArray() {
        // print out the results
        for (int i = 0; i < puzzleSize; i++) {
            for (int j = 0; j< puzzleSize; j++) {
                std::cout<<state[i][j];
            }
            std::cout<<std::endl;
        }
        std::cout<<"-------------"<<std::endl;
    }
};
class Puzzle {
    int maxPathCost = 0;
    int calculateMaxPathCost() {
        int value = 0;
        for (int i = 0; i < puzzleSize; i++) {
            for (int j = 0; j < puzzleSize; j++) {
                for (int k = 0; k < puzzleSize; k++) {
                    for (int l = 0; l < puzzleSize; l++) {
                        if (goalState[k][l] == initialState[i][j] && goalState[k][l] != blankSquare) {
                            int newPathCost = abs(i - k) + abs(j - l);
                            value += newPathCost;
                        }
                    }
                }
            }
        }
        return value;
    }
public:
    void doPuzzle(State currentState, std::vector<State> stateList) {
        if (stateList.size() >= maxPathCost) {
            // stop recusive
            for (State s: stateList) {
                s.printArray();
            }
            std::cout<<"==================="<<std::endl;
            return;
        }
        for (int i = Left; i <= Down; i++) {
            State state = currentState.clone();
            Actions a = static_cast<Actions>(i);
            int c = state.doAction(a);
            if (c > 0) {
                stateList.insert(stateList.end(), state);
                doPuzzle(state, stateList);
            }
        }
    }
    void puzzle(){
        maxPathCost = calculateMaxPathCost();
        doPuzzle(State(initialState), {});
    }
};
int main(int argc, const char * argv[]) {
    Puzzle puzzle = Puzzle();
    puzzle.puzzle();
    return 0;
}

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using namespace std;
//region defs

void wait();
void block();

enum Direction { Left, Right };
Direction parseDirection(const string&);
Direction positionDirection(int, int);

//endregion
// semantically these are constants, hence the name
//region vars

int kNbFloors; // number of floors
int kWidth; // width of the area
int kRoundsCount; // maximum number of rounds
int kExitFloor; // floor on which the exit is found
int kExitPos; // position of the exit on its floor
int kClonesCount; // number of generated clones
int __ignored; // ignore (always zero)
int kElevatorsCount; // number of elevators
//endregion
//region struct

struct Clone {
    int floor;
    int position;
    Direction direction;
    // will outlive us for sure
    unordered_map<int, int>& elevators_positions;

    bool isOnExitFloor() {
        return floor == kExitFloor;
    }

    bool isFacingExit() {
        return direction == positionDirection(position, kExitPos);
    }

    bool isFacingElevator() {
        int elevator_pos = elevators_positions[floor];
        return direction == positionDirection(position, elevator_pos);
    }
};

void readClone(Clone& clone) {
    string direction; // direction of the leading clone: LEFT or RIGHT
    cin >> clone.floor >> clone.position >> direction;
    cin.ignore();

    clone.direction = parseDirection(direction);
}

//endregion


int main()
{
    cin >> kNbFloors >> kWidth >> kRoundsCount >> kExitFloor >> kExitPos >> kClonesCount >> __ignored >> kElevatorsCount; cin.ignore();

    unordered_map<int, int> elevators_positions;

    for (int i = 0; i < kElevatorsCount; i++) {
        int elevator_floor; // floor on which this elevator is found
        int elevator_pos; // position of the elevator on its floor
        cin >> elevator_floor >> elevator_pos; cin.ignore();

        elevators_positions.insert(elevator_floor, elevator_pos);
    }

    Clone leading_clone{};
    leading_clone.elevators_positions = elevators_positions;
    while (true) {
        readClone(leading_clone);

        if(leading_clone.floor < 0) {
            wait();
            continue;
        }

        if(leading_clone.isOnExitFloor()) {
            if(leading_clone.isFacingExit()) {
                wait();
            } else {
                block();
            }
            continue;
        }

        if(leading_clone.isFacingElevator()) {
            wait();
        } else {
            block();
        }
    }
}


Direction positionDirection(int clone_pos, int position) {
    assert(clone_pos != position);
    return position > clone_pos ?
           Direction::Right : Direction::Left;
}

Direction parseDirection(const string& dir) {
    return dir == "RIGHT" ?
        Direction::Right : Direction::Left;
}

void wait() {
    cout << "WAIT" << endl;
}

void block() {
    cout << "BLOCK" << endl;
}



#pragma clang diagnostic pop
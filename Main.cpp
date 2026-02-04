#include <iostream>
#include <string>
#include <vector>

using namespace std;

// -----------------------------
// Domain Models
// -----------------------------
struct Patient {
    int id;
    string name;
    int age;
    string gender;
    string diagnosis;

    Patient() : id(0), age(0) {}

    void print() const {
        cout << "Patient[ID=" << id
             << ", Name=" << name
             << ", Age=" << age
             << ", Gender=" << gender
             << ", Dx=" << diagnosis << "]";
    }
};

struct Bed {
    int bedId;
    string ward;
    string room;
    bool occupied;

    // Instead of optional<Patient>
    bool hasPatient;
    Patient patient;

    Bed() : bedId(0), occupied(false), hasPatient(false) {}

    void print() const {
        cout << "Bed[BedID=" << bedId
             << ", Ward=" << ward
             << ", Room=" << room
             << ", Status=" << (occupied ? "OCCUPIED" : "AVAILABLE");

        if (occupied && hasPatient) {
            cout << ", ";
            patient.print();
        }
        cout << "]";
    }
};

// -----------------------------
// Service Layer
// -----------------------------
class BedManager {
private:
    vector<Bed> beds;

    Bed* findBedById(int bedId) {
        for (size_t i = 0; i < beds.size(); i++) {
            if (beds[i].bedId == bedId) return &beds[i];
        }
        return NULL;
    }

    Bed* findAvailableBed(const string& ward, const string& room) {
        for (size_t i = 0; i < beds.size(); i++) {
            Bed &b = beds[i];
            if (!b.occupied &&
                (ward.empty() || b.ward == ward) &&
                (room.empty() || b.room == room)) {
                return &b;
            }
        }
        return NULL;
    }

public:
    bool addBed(int bedId, const string& ward, const string& room) {
        if (findBedById(bedId) != NULL) return false;

        Bed b;
        b.bedId = bedId;
        b.ward = ward;
        b.room = room;
        b.occupied = false;
        b.hasPatient = false;

        beds.push_back(b);
        return true;
    }

    // Admission: returns true and outputs assigned bedId via reference
    bool admitPatient(const Patient& p, int &assignedBedId,
                      const string& preferredWard = "", const string& preferredRoom = "") {

        // Ensure patient isn't already admitted
        for (size_t i = 0; i < beds.size(); i++) {
            Bed &b = beds[i];
            if (b.occupied && b.hasPatient && b.patient.id == p.id) {
                return false;
            }
        }

        Bed* bed = findAvailableBed(preferredWard, preferredRoom);
        if (!bed) return false;

        bed->occupied = true;
        bed->hasPatient = true;
        bed->patient = p;
        assignedBedId = bed->bedId;
        return true;
    }

    bool dischargePatient(int patientId) {
        for (size_t i = 0; i < beds.size(); i++) {
            Bed &b = beds[i];
            if (b.occupied && b.hasPatient && b.patient.id == patientId) {
                b.occupied = false;
                b.hasPatient = false;
                b.patient = Patient(); // reset
                return true;
            }
        }
        return false;
    }

    // Transfer: returns true and outputs new bedId
    bool transferPatient(int patientId, int &newBedId,
                         const string& newWard = "", const string& newRoom = "") {

        Bed* currentBed = NULL;
        for (size_t i = 0; i < beds.size(); i++) {
            Bed &b = beds[i];
            if (b.occupied && b.hasPatient && b.patient.id == patientId) {
                currentBed = &b;
                break;
            }
        }
        if (!currentBed) return false;

        Bed* targetBed = findAvailableBed(newWard, newRoom);
        if (!targetBed) return false;

        // move patient
        targetBed->occupied = true;
        targetBed->hasPatient = true;
        targetBed->patient = currentBed->patient;

        currentBed->occupied = false;
        currentBed->hasPatient = false;
        currentBed->patient = Patient();

        newBedId = targetBed->bedId;
        return true;
    }

    // Search: returns true, outputs patient + bedId
    bool findPatient(int patientId, Patient &outPatient, int &outBedId) const {
        for (size_t i = 0; i < beds.size(); i++) {
            const Bed &b = beds[i];
            if (b.occupied && b.hasPatient && b.patient.id == patientId) {
                outPatient = b.patient;
                outBedId = b.bedId;
                return true;
            }
        }
        return false;
    }

    void listAllBeds() const {
        if (beds.empty()) {
            cout << "No beds in the system.\n";
            return;
        }
        for (size_t i = 0; i < beds.size(); i++) {
            beds[i].print();
            cout << "\n";
        }
    }

    void listAvailableBeds() const {
        bool any = false;
        for (size_t i = 0; i < beds.size(); i++) {
            if (!beds[i].occupied) {
                beds[i].print();
                cout << "\n";
                any = true;
            }
        }
        if (!any) cout << "No available beds.\n";
    }

    void listOccupiedBeds() const {
        bool any = false;
        for (size_t i = 0; i < beds.size(); i++) {
            if (beds[i].occupied) {
                beds[i].print();
                cout << "\n";
                any = true;
            }
        }
        if (!any) cout << "No occupied beds.\n";
    }
};

// -----------------------------
// Simple Console UI helpers
// -----------------------------
static int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        getline(cin, s);
        // simple parse
        bool ok = !s.empty();
        for (size_t i = 0; i < s.size(); i++) {
            if ((i == 0 && (s[i] == '-' || s[i] == '+')) || (s[i] >= '0' && s[i] <= '9')) continue;
            ok = false;
            break;
        }
        if (ok) return atoi(s.c_str());
        cout << "Invalid number. Try again.\n";
    }
}

static string readStr(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

static void printMenu() {
    cout << "\n========== Bed Management System ==========\n"
         << "1) Add Bed\n"
         << "2) Admit Patient\n"
         << "3) Discharge Patient\n"
         << "4) Transfer Patient\n"
         << "5) Search Patient\n"
         << "6) List All Beds\n"
         << "7) List Available Beds\n"
         << "8) List Occupied Beds\n"
         << "0) Exit\n"
         << "==========================================\n";
}

int main() {
    BedManager manager;

    // Sample beds
    manager.addBed(101, "Ward-A", "Room-1");
    manager.addBed(102, "Ward-A", "Room-1");
    manager.addBed(201, "Ward-B", "Room-2");

    while (true) {
        printMenu();
        int choice = readInt("Choose an option: ");

        if (choice == 0) {
            cout << "Goodbye.\n";
            break;
        }

        if (choice == 1) {
            int bedId = readInt("Bed ID: ");
            string ward = readStr("Ward: ");
            string room = readStr("Room: ");
            if (manager.addBed(bedId, ward, room)) cout << "Bed added.\n";
            else cout << "Bed ID already exists.\n";
        }
        else if (choice == 2) {
            Patient p;
            p.id = readInt("Patient ID: ");
            p.name = readStr("Name: ");
            p.age = readInt("Age: ");
            p.gender = readStr("Gender: ");
            p.diagnosis = readStr("Diagnosis: ");

            string ward = readStr("Preferred Ward (or leave blank): ");
            string room = readStr("Preferred Room (or leave blank): ");

            int assignedBedId = -1;
            if (manager.admitPatient(p, assignedBedId, ward, room))
                cout << "Admitted. Assigned Bed ID: " << assignedBedId << "\n";
            else
                cout << "Admission failed (no suitable bed, or patient already admitted).\n";
        }
        else if (choice == 3) {
            int pid = readInt("Patient ID to discharge: ");
            if (manager.dischargePatient(pid)) cout << "Discharged and bed freed.\n";
            else cout << "Patient not found.\n";
        }
        else if (choice == 4) {
            int pid = readInt("Patient ID to transfer: ");
            string ward = readStr("New Ward preference (or leave blank): ");
            string room = readStr("New Room preference (or leave blank): ");

            int newBedId = -1;
            if (manager.transferPatient(pid, newBedId, ward, room))
                cout << "Transferred. New Bed ID: " << newBedId << "\n";
            else
                cout << "Transfer failed (patient not found or no suitable bed).\n";
        }
        else if (choice == 5) {
            int pid = readInt("Patient ID to search: ");
            Patient outP;
            int bedId = -1;
            if (manager.findPatient(pid, outP, bedId)) {
                cout << "Found in Bed ID: " << bedId << "\n";
                outP.print();
                cout << "\n";
            } else {
                cout << "Patient not found.\n";
            }
        }
        else if (choice == 6) manager.listAllBeds();
        else if (choice == 7) manager.listAvailableBeds();
        else if (choice == 8) manager.listOccupiedBeds();
        else cout << "Unknown option.\n";
    }

    return 0;
}

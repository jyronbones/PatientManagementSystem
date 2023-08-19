// FinalProject-040585687.cpp : Defines the entry point for the application.
//

#include "FinalProject-040585687.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <vector>
#include <string>
#include<cstdlib>
#include<ctime>
#include <algorithm>
#include <iterator>

using namespace std;

class Record {
public:
    string date;
    bool infected;
};
class Patient {
public:
    string name;
    int age;
    string postalCode;
    vector<Record> record;
};


int main(int argc, const char* argv[]) {
    static vector<Patient> patients;
    static int selection;

    sf::RenderWindow window{ {800, 600},"TGUI window with sfml" };
    tgui::Gui gui{ window };
    gui.loadWidgetsFromFile("form.txt");

    /* labels */
    static auto covid_test_label = gui.get<tgui::Label>("covid_test_label");
    covid_test_label->setEnabled(false);
    static auto date_label = gui.get<tgui::Label>("date_label");
    date_label->setEnabled(false);
    /* child windows */
    auto stats_window = gui.get<tgui::ChildWindow>("stats_window");
    auto hotspot_window = gui.get<tgui::ChildWindow>("hotspot_window");
    hotspot_window->setVisible(false);
    stats_window->setVisible(false);
    /* buttons */
    auto submit_info_btn = gui.get<tgui::Button>("submit_info_btn");
    static auto submit_test_btn = gui.get<tgui::Button>("submit_test_btn");
    submit_test_btn->setEnabled(false);
    auto covid_test_btn = gui.get<tgui::Button>("covid_test_btn");
    static auto delete_patient_btn = gui.get<tgui::Button>("delete_patient_btn");
    delete_patient_btn->setEnabled(false);
    auto calculate_stats_btn = gui.get<tgui::Button>("calculate_stats_btn");
    auto calculate_hotspots_btn = gui.get<tgui::Button>("calculate_hotspots_btn");
    auto stats_close_btn = gui.get<tgui::Button>("stats_close_btn");
    auto hotspot_close_btn = gui.get<tgui::Button>("hotspot_close_btn");

    /* text areas */
    auto patientName = gui.get<tgui::TextArea>("name_text");
    auto patientAge = gui.get<tgui::TextArea>("age_text");
    auto postalCode = gui.get<tgui::TextArea>("postal_text");

    /* comboboxes */
    static auto month_combobox = gui.get<tgui::ComboBox>("month_combobox");
    month_combobox->setEnabled(false);
    static auto day_combobox = gui.get<tgui::ComboBox>("day_combobox");
    day_combobox->setEnabled(false);
    static auto year_combobox = gui.get<tgui::ComboBox>("year_combobox");
    year_combobox->setEnabled(false);

    /* list views */
    auto listBox = gui.get<tgui::ListView>("patient_listbox");
    static auto testBox = gui.get<tgui::ListView>("covid_listview");
    testBox->setEnabled(false);
    auto statsBox = gui.get<tgui::ListView>("stats_listview");
    auto hotspotBox = gui.get<tgui::ListView>("hotspot_listview");
    // listBox ->addItem("Hello world");
    listBox->onItemSelect([](int index) {
        selection = index;
        cout << "You selected index:" << index << endl;
        testBox->removeAllItems();
        delete_patient_btn->setEnabled(true);
        if (selection != -1) {
            for (Record r : patients[selection].record) {
                string recordText;
                if (r.infected == true) {
                    recordText = r.date + "   " + "POSITIVE";
                }
                else {
                    recordText = r.date + "   " + "NEGATIVE";
                }
                testBox->addItem(recordText);
            }
        }
        });


    /* lambda functions */
    submit_info_btn->onClick([&]() { //& to access a variable from outside the lambda function (&patientName)
        
        auto name = patientName->getText();
        auto age = patientAge->getText();
        auto postal = postalCode->getText();
        auto patient_info = name + "   " + age + "   " + postal;
        listBox->addItem(patient_info);

        Patient patient;
        patient.name = (string)name;
        string pAge = (string)age;
        patient.age = stoi(pAge);
        patient.postalCode = (string)postal;
        patients.push_back(patient);
         });

    submit_test_btn->onClick([&]() { //& to access a variable from outside the lambda function (&patientName)
        Record patientRecord;
        string recordDate;
        bool isInfected;
        string month = (string)month_combobox->getSelectedItem();
        auto day = (string)day_combobox->getSelectedItem();
        auto year = (string)year_combobox->getSelectedItem();
        string date = month + " " + day + " " + year;
        string testResult;

        int r = rand() % (20) + 1;
        if (r == 13) {
            recordDate = date;
            isInfected = true;
            testResult = date + "   " + "POSITIVE";
        }
        else {
            recordDate = date;
            isInfected = false;
            testResult = date + "   " + "NEGATIVE";
        }
        patientRecord.date = recordDate;
        patientRecord.infected = isInfected;
        patients[selection].record.push_back(patientRecord);
        testBox->addItem(testResult);

        });

    covid_test_btn->onClick([&]() { //& to access a variable from outside the lambda function (&patientName)
        submit_test_btn->setEnabled(true);
        covid_test_label->setEnabled(true);
        covid_test_label->setEnabled(true);
        month_combobox->setEnabled(true);
        day_combobox->setEnabled(true);
        year_combobox->setEnabled(true);
        testBox->setEnabled(true);
        });
    delete_patient_btn->onClick([&]() {
        patients.erase(patients.begin() + selection);
        listBox->removeItem(selection);
        submit_test_btn->setEnabled(false);
        submit_test_btn->setEnabled(false);
        covid_test_label->setEnabled(false);
        covid_test_label->setEnabled(false);
        month_combobox->setEnabled(false);
        day_combobox->setEnabled(false);
        year_combobox->setEnabled(false);
        testBox->setEnabled(false);
        if (selection == -1) {
            delete_patient_btn->setEnabled(false);
        }
        });

    calculate_stats_btn->onClick([&]() {
        stats_window->setVisible(true);
        auto highestAge = 0;
        std::map<int, int> ageMap;

        for (Patient p : patients) {
            if (p.age > highestAge) {
                highestAge = p.age;
            }
            int ageGroup = p.age / 5;
            ageMap.insert(make_pair(ageGroup, 1));
            auto ageRange = ageMap.find(ageGroup);
            if (ageRange != ageMap.end()) {
                for (Record r : p.record) {
                    if (r.infected == true) {
                        ageRange->second++;
                    }
                }
            }
        }
        for (auto kv : ageMap) {
            string ageResult = "Age group " + to_string(kv.first * 5) + "-" + to_string(kv.first * 5 + 4) + ":" + "              " + to_string(kv.second - 1) + " case(s)";
            statsBox->addItem(ageResult);
        }
        });

    calculate_hotspots_btn->onClick([&]() {
        hotspot_window->setVisible(true);
        std::map<string, int> hotspotMap;
        for (Patient p : patients) {
            string postal = p.postalCode.substr(0, 3);

            hotspotMap.insert(make_pair(postal, 0));
            auto thisPostal = hotspotMap.find(postal);
            if (thisPostal != hotspotMap.end()) {
                for (Record r : p.record) {
                    if (r.infected == true) {
                        thisPostal->second++;
                    }
                }
            }
        }
        for (auto kv : hotspotMap) {
            string hotResult = "Postal code: " + kv.first + ", " + to_string(kv.second) + " case(s)";
            hotspotBox->addItem(hotResult);

        }

        });

    stats_close_btn->onClick([&]() {
        stats_window->setVisible(false);
        statsBox->removeAllItems();
        });
    hotspot_close_btn->onClick([&]() {
        hotspot_window->setVisible(false);
        hotspotBox->removeAllItems();
        });

    gui.mainLoop();

    return 0;
}

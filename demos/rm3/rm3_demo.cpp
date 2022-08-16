#include "../../src/hydro_forces.h"
#include "chrono_irrlicht/ChVisualSystemIrrlicht.h"
#include "chrono_irrlicht/ChIrrMeshTools.h"
// Use the namespaces of Chrono
using namespace chrono;
using namespace chrono::geometry;
using namespace chrono::irrlicht;

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::io;
using namespace irr::gui;

//// =============================================================================
//class MyEventReceiver : public IEventReceiver {
//public:
//	MyEventReceiver(ChVisualSystemIrrlicht* myapp, bool& buttonPressed)
//		: pressed(buttonPressed) {
//		// store pointer application
//		application = myapp;
//
//		// ..add a GUI button to control pause/play
//		//pauseButton = application->GetIGUIEnvironment()->addButton(rect<s32>(510, 20, 650, 35));
//		//buttonText = application->GetIGUIEnvironment()->addStaticText(L"Paused", rect<s32>(560, 20, 600, 35), false);
//	}
//
//	bool OnEvent(const SEvent& event) {
//		// check if user clicked button
//		if (event.EventType == EET_GUI_EVENT) {
//			switch (event.GUIEvent.EventType) {
//			case EGET_BUTTON_CLICKED:
//				pressed = !pressed;
//				if (pressed) {
//					buttonText->setText(L"Playing");
//				}
//				else {
//					buttonText->setText(L"Paused");
//				}
//				return pressed;
//				break;
//			default:
//				break;
//			}
//		}
//		return false;
//	}
//
//private:
//	ChVisualSystemIrrlicht* application;
//	IGUIButton* pauseButton;
//	IGUIStaticText* buttonText;
//
//	bool& pressed;
//};

int main(int argc, char* argv[]) {
	//auto start = std::chrono::high_resolution_clock::now();
	GetLog() << "Copyright (c) 2017 projectchrono.org\nChrono version: " << CHRONO_VERSION << "\n\n";

	ChSystemNSC system;
	system.Set_G_acc(ChVector<>(0, 0, -9.81));
	// Create the Irrlicht application for visualizing
	//ChApiIrr application;// (&system, L"MultiBody Demo", core::dimension2d<u32>(800, 600));
	auto application = chrono_types::make_shared<ChVisualSystemIrrlicht>();
	
	application->AttachSystem(&system);
	application->SetWindowSize(800, 600);
	application->SetWindowTitle("MultiBody Demo");
	application->SetCameraVertical(CameraVerticalDir::Y);
	application->Initialize();
	application->AddLogo();
	application->AddSkyBox();
	application->AddCamera(ChVector<>(0, 0, -6), ChVector<>(-2, 2, 0));
	application->AddTypicalLights();

	//application.SetCameraVertical(CameraVerticalDir::Z)
	//application.AddLogo();
	//application.AddSkyBox();
	//application.AddTypicalLights();
	//application.AddCamera(core::vector3df(0, 70, -10), core::vector3df(0, 0, -10)); // arguments are (location, orientation) as vectors

	// uncomment out lines for xy plane on z=0 to be shown
	//auto ground = chrono_types::make_shared<ChBodyEasyBox>(50, 50, 0.05, 10, true, false);
	//system.AddBody(ground);
	//ground->SetBodyFixed(true);
	//ground->SetCollide(false);
	//ground->SetPos(chrono::ChVector(0, 0, 0));


	// set up body from a mesh
	if (!std::filesystem::exists("../../HydroChrono/meshFiles/float.obj")) {
		std::cout << "File " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/meshFiles/float.obj").c_str()) << " does not exist" << std::endl;
		return 0;
	}
	//std::cout << "Attempting to open mesh file: " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/meshFiles/float.obj").c_str()) << std::endl;
	std::shared_ptr<ChBody> float_body1 = chrono_types::make_shared<ChBodyEasyMesh>(                   //
		GetChronoDataFile("../../HydroChrono/meshFiles/float.obj").c_str(),                 // file name
		1000,                                                                                     // density
		false,                                                                                    // do not evaluate mass automatically
		true,                                                                                     // create visualization asset
		false,                                                                                    // do not collide
		nullptr,                                                                                  // no need for contact material
		0                                                                                         // swept sphere radius
		);

	// set up body from a mesh
	if (!std::filesystem::exists("../../HydroChrono/meshFiles/plate.obj")) {
		std::cout << "File " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/meshFiles/plate.obj").c_str()) << " does not exist" << std::endl;
		return 0;
	}
	//std::cout << "Attempting to open mesh file: " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/meshFiles/plate.obj").c_str()) << std::endl;
	std::shared_ptr<ChBody> plate_body2 = chrono_types::make_shared<ChBodyEasyMesh>(                   //
		GetChronoDataFile("../../HydroChrono/meshFiles/plate.obj").c_str(),                 // file name
		1000,                                                                                     // density
		false,                                                                                                                                                                                // do not evaluate mass automatically
		true,                                                                                     // create visualization asset
		false,                                                                                    // do not collide
		nullptr,                                                                                  // no need for contact material
		0                                                                                         // swept sphere radius
		);

	// set up body initial conditions
	system.Add(float_body1);
	float_body1->SetNameString("body1"); 
	//float_body1->SetPos(ChVector<>(0, 0, 0));
	float_body1->SetMass(886.691);
	float_body1->SetCollide(false);
	//// attach color asset to body
	//auto col_1 = chrono_types::make_shared<ChColorAsset>();
	//col_1->SetColor(ChColor(0, 0, 0.6f));
	//float_body1->AddAsset(col_1);

	// set up body initial conditions
	system.Add(plate_body2);
	plate_body2->SetNameString("body2");
	//plate_body2->SetPos(ChVector<>(0, 0, 0));
	plate_body2->SetMass(886.691);
	plate_body2->SetCollide(false);
	// attach color asset to body
	//auto col_2 = chrono_types::make_shared<ChColorAsset>();
	//col_2->SetColor(ChColor(0, 0.7f, 0.8f));
	//plate_body2->AddAsset(col_2);

	// add constraint so they only move up and down! TODO

	HydroInputs my_hydro_inputs;
	my_hydro_inputs.SetRegularWaveAmplitude(0.022);
	my_hydro_inputs.SetRegularWaveOmega(2.10);

	std::vector<std::shared_ptr<ChBody>> bodies;
	bodies.push_back(float_body1);
	bodies.push_back(plate_body2);
	TestHydro blah(bodies, "C:/Users/ZQUINTON/code/HydroChrono/rm3.h5", my_hydro_inputs);
	//std::shared_ptr<ChLoadContainer> my_loadcontainer;
	//std::shared_ptr<ChLoadAddedMass> my_loadbodyinertia;
	//my_loadcontainer = chrono_types::make_shared<ChLoadContainer>();
	//ChMatrixDynamic<> amMatrix;

	//my_loadbodyinertia = chrono_types::make_shared<ChLoadAddedMass>(amMatrix, bodies);

	//system.Add(my_loadcontainer);
	//my_loadcontainer->Add(my_loadbodyinertia);


	//// update irrlicht app with body info
	//application.AssetBindAll();
	//application.AssetUpdateAll();

	//// some tools to handle the pause button
	//bool buttonPressed = false;
	//MyEventReceiver receiver(&application, buttonPressed);
	//application.SetUserEventReceiver(&receiver);

	// Info about which solver to use - may want to change this later

	//auto gmres_solver = chrono_types::make_shared<ChSolverGMRES>();  // change to mkl or minres?
	//gmres_solver->SetMaxIterations(300);
	//system.SetSolver(gmres_solver);
	//double timestep = 0.06; // also sets the timesteps in chrono system
	//application.SetTimestep(timestep);

	//MyEventReceiver receiver(application.get());
	// note how to add the custom event receiver to the default interface:
	//application->AddUserEventReceiver(&receiver);

	// Solver settings
	system.SetSolverType(ChSolver::Type::GMRES);
	system.SetSolverMaxIterations(300);  // the higher, the easier to keep the constraints satisfied.
	system.SetStep(0.06);

	// Simulation loop
	double timestep = 0.03;
	//ChRealtimeStepTimer realtime_timer;
	while (application->Run() && system.GetChTime() <= 40) {
		application->BeginScene();
		application->Render();
		tools::drawGrid(application.get(), 2, 2, 30, 30, ChCoordsys<>(ChVector<>(0, 0.01, 0), Q_from_AngX(CH_C_PI_2)),
			ChColor(0.3f, 0.3f, 0.3f), true);

		application->EndScene();
		system.DoStepDynamics(timestep);
		//realtime_timer.Spin(timestep);
	}

	// TODO compare solvers?
	//auto gmres_solver = chrono_types::make_shared<ChSolverGMRES>();  // change to mkl or minres?
	//gmres_solver->SetMaxIterations(300);
	//system.SetSolver(gmres_solver);
	//double timestep = 0.06; // also sets the timesteps in chrono system
	//application.SetTimestep(timestep);


	// set up output file for body position each step
	std::string of = "output.txt";                    /// < put name of your output file here
	std::ofstream zpos(of, std::ofstream::out);
	if (!zpos.is_open()) {
		std::cout << "Error opening file \"" + of + "\". Please make sure this file path exists then try again\n";
		return -1;
	}	
	std::cout << "Writing positions to file: " << std::filesystem::absolute(of) << std::endl;
	zpos.precision(10);
	zpos.width(12);
	zpos << "#Time\tHeave " << float_body1->GetNameString() << "\t" << plate_body2->GetNameString() << std::endl;
	//zpos << "#Time\tBody vel" << std::endl;


	// Simulation loop
	int frame = 0;

	while (application->Run() && system.GetChTime() < 3) {
		application->BeginScene();
		application->Render();
		tools::drawAllCOGs(application.get(), 15); // draws all cog axis lines, kinda neat
		//tools::drawGrid(application.GetVideoDriver(), 4, 4);
		/*if (buttonPressed)*/if(true) {
			zpos << system.GetChTime() << "\t" << float_body1->GetPos_dt().x() << "\t" << float_body1->GetPos_dt().y() << "\t" << float_body1->GetPos_dt().z();
			zpos << "\t" << float_body1->GetWvel_par().x() << "\t" << float_body1->GetWvel_par().y() << "\t" << float_body1->GetWvel_par().z() << std::endl;
			zpos << system.GetChTime() << "\t" << plate_body2->GetPos_dt().x() << "\t" << plate_body2->GetPos_dt().y() << "\t" << plate_body2->GetPos_dt().z();
			zpos << "\t" << plate_body2->GetWvel_par().x() << "\t" << plate_body2->GetWvel_par().y() << "\t" << plate_body2->GetWvel_par().z() << std::endl;
			system.DoStepDynamics(timestep);

			frame++;
		}
		application->EndScene();
	}
	zpos.close();
	//auto end = std::chrono::high_resolution_clock::now();
	//unsigned duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	//std::cout << "Duration: " << duration/1000.0 << " seconds" << std::endl;
	return 0;
}
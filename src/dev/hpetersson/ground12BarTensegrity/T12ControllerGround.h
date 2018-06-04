/*
 * Copyright © 2012, United States Government, as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All rights reserved.
 * 
 * The NASA Tensegrity Robotics Toolkit (NTRT) v1 platform is licensed
 * under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
l */

#ifndef T12CONTROLLERGROUND
#define T12CONTROLLERGROUND

/**
 * @file T12ControllerGround.h
 * @brief Contains the definition of class T12ControllerGround.
 * @author Hannah Petersson based on code from Steven Lessard
 * @version 1.0.0
 * $Id$
 */

#include <vector>

#include "core/tgObserver.h"
#include "learning/Adapters/AnnealAdapter.h"

// Forward declarations
class T12ModelGround;
class tgBasicActuator;

//namespace std for vectors
using namespace std;

/** Escape Controller for T12` */
class T12ControllerGround : public tgObserver<T12ModelGround>
{
    public:
        // Note that currently this is calibrated for decimeters.
        T12ControllerGround(T12ModelGround* subject, const double prefLength=5.0, double startTime=3);

        /** Nothing to delete, destructor must be virtual */
        virtual ~T12ControllerGround() { }

	void getFileName(void);

        virtual void onSetup(T12ModelGround& subject);

        virtual void onStep(T12ModelGround& subject, double dt);

        virtual void onTeardown(T12ModelGround& subject);
    protected:
        virtual vector< vector <double> > transformActions(vector< vector <double> > act);

        virtual void applyActions(T12ModelGround& subject, vector< vector <double> > act);

    private:
        vector<double> initPosition; // Initial position of model
        const double m_initialLengths;
   	double m_startTime;
        double m_totalTime;
        double const maxStringLengthFactor; // Proportion of string's initial length by which a given actuator can increase/decrease

        // Evolution and Adapter
        AnnealAdapter evolutionAdapter;
        vector< vector<double> > actions; // For modifications between episodes

        // Muscle Clusters
        int nSquareClusters;
	int nHexaClusters;
	int musclesPerSquareCluster;
        int musclesPerHexaCluster;

        /** Two vectors clusters, each of which contains a vector of muscles */
        vector<vector<tgBasicActuator*> > squareClusters;  
        vector<vector<tgBasicActuator*> > hexaClusters; 

        // Sine Wave Data
        double* retractRate;
        double* elongateRate;

        /** Initialize the evolution adapter as well as its own parameters */
        void setupAdapter();

        /** Returns amount of (double content);energy spent by each muscle in subject */
        double totalEnergySpent(T12ModelGround& subject);
	double energySpent;

        /** Sets target lengths for each muscle */
        void setPreferredMuscleLengths(T12ModelGround& subject, double dt, double oldCluster, double currentCluster);

        /** Divides the 24 muscles of an Escape_T6Model 
         * into 8 clusters of 3 muscles */
        void populateClusters(T12ModelGround& subject);

        /** Sets the amplitude, angularFrequency, phase, and dcOffset 
         * for each sine wave used in muscle actuation */
        void initializeRates();

        /** Difference in position between initPosition and finalPosition
         * of subject */
        double displacement(T12ModelGround& subject);
	double distanceMovedManhattan;
	double distanceMovedSnorkel;

        /** Select action paramters from a comma-separated line in a file */
        std::vector<double> readManualParams(int lineNumber, const char* filename);

        void printRates();

 	double getGroundFace(T12ModelGround& subject);

  	double getAdjacentFace(void);

	/* For a certain rod configuration, determine corresponding face */
	void determineFace(bool isSquareFace);

	vector<double> groundRods; // for checking which face is on the ground
        double groundFace; // Current face on ground
        vector<double> groundFaceHistory; // Vector containing all faces touching ground

     	void write2txtFile(double contentDouble, char const* contentString, bool isDouble);
        void write2csvFile(double contentDouble, char const* sign, bool isDouble);

  	string txtPath;  // file name for txt file
	string csvPath;  // file name for csv file

	void saveData2File(void);

	void clearParams(void);
	double simulationNumber;
};

#endif // T12CONTROLLERGROUND

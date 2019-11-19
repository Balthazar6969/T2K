#ifndef TTruthUtils_h_seen
#define TTruthUtils_h_seen


#include <string>
#include <vector>
#include <TClonesArray.h>
//#include "libReadoaAnalysis/libReadoaAnalysisProjectHeaders.h"
#include "Versioning.hxx"
#include <algorithm>

/// Analysis utilities for the Truth Trajecotries and Vertices modules
class TTruthUtils
{
	public:

	/// You need to provide the class with pointers to the variables
		/// into which you read the Trajecotries and Vertices TClonesArrays,
		/// and the variables into which you read the number of entries
		/// in each clones array.
		///
		/// If you are not using one of the Trajectories/Vertices, you can
		/// safely pass 0 to the relevant arguments, and the class will always
		/// check before using them.
		///
		/// At any time, the pointers can be changed/added using appropriate
		/// methods provided
		TTruthUtils(TClonesArray* trajectories, TClonesArray* vertices, Int_t* n_trajectories, Int_t* n_vertices);
		TTruthUtils();
	
		~TTruthUtils(){};
		
		/// Change the location of the TClonesArray of vertices
		void SetVerticesArray(TClonesArray* vertices)	{	fVertices = vertices;	}
		
		/// Change the location of the TClonesArray of vertices
		void SetVerticesNumber(Int_t* n_vertices)	{	fNVertices = n_vertices;	}
		
		/// Change the location of the TClonesArray of trajectories
		void SetTrajectoriesArray(TClonesArray* trajectories);	
	
		/// Change the location of the TClonesArray of trajectories
		void SetTrajectoriesNumber(Int_t* n_trajectories);
	
	
		/*
		 * Get Trajectories
		 */
		
		/// Return a pointer to the trajectory with a given ID.
		/// If trajectory is not saved, return NULL.
		ND::TTruthTrajectoriesModule::TTruthTrajectory* GetTrajectoryById(Int_t id) const;
		
		/// Get the first parent of a trajectory.
		/// If the trajectory has no parent, return NULL (ie: it is a primary trajectory).
		/// If the parent is not stored, return NULL.
		ND::TTruthTrajectoriesModule::TTruthTrajectory* GetParentTrajectory(Int_t child_id) const;
		ND::TTruthTrajectoriesModule::TTruthTrajectory* GetParentTrajectory(ND::TTruthTrajectoriesModule::TTruthTrajectory* child_trajectory) const;
		
		/// Get the primary parent of a trajectory.
		/// If the trajectory has no primary parent, return NULL (ie: it is a primary trajectory).
		/// If the primary parent is not stored, return NULL (the primary trajectories are always stored)
		ND::TTruthTrajectoriesModule::TTruthTrajectory* GetPrimaryParentTrajectory(Int_t child_id) const;
		ND::TTruthTrajectoriesModule::TTruthTrajectory* GetPrimaryParentTrajectory(ND::TTruthTrajectoriesModule::TTruthTrajectory* child_trajectory) const;
		
		/// Get trajectories sorted by momentum.
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetMomentumSortedTrajectories() const;
		
		/// Get trajectories of a specific particle category.
		/// The integer values specifying the particle type are defined
		/// in oaAnalysis ND::ToaAnalysisUtils::EParticleCategory.
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByCategory(int category) const;
		

#if    !VERSION_HAS_REMOVED_OBSOLETE_TRUTH_SUBDETECTOR_NON_TRACE_VARS

    /// Get trajectories by final detector.
		/// The integer values specifying the detector are defined in
		/// oaAnalysis ND::ToaAnalysisUtils::ESubdetector
    
      
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByFinalSubdetector(int final_subdetector) const;
		
		/// Get trajectories by intial detector.
		/// The integer values specifying the detector are defined in
		/// oaAnalysis ND::ToaAnalysisUtils::ESubdetector.
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByInitSubdetector(int init_subdetector) const;

#endif
    
		/// Get all trajectories with a specific particle name.
		/// eg: e-, mu-, proton, neutron.
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByName(std::string name) const;
		
		/// Get all trajectories with a specific PDG code
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByPDG(Int_t pdg) const;
		
		/// Get trajectories with a specific parent.
		/// This will not include the parent itself.
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByParent(Int_t parent_id) const;
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByParent(ND::TTruthTrajectoriesModule::TTruthTrajectory* parent) const;
		
		/// Get all trajectories descending from a primary trajectory.
		/// This will include the primary trajectory itself.
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByPrimaryParent(Int_t primary_parent_id) const;
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesByPrimaryParent(ND::TTruthTrajectoriesModule::TTruthTrajectory* primary_parent) const;
		
#if    !VERSION_HAS_REMOVED_OBSOLETE_TRUTH_SUBDETECTOR_NON_TRACE_VARS
		/// Get trajectories which have true values for EnteredSubdetector
		/// or ExitedSubdetector.
		/// See the oaAnalysis TTruthTrajectories class for full
		/// documentation on those fields' precise meanings.
		/// Essentially, it will return trajectories which entered or
		/// exited the active region of the specified subdetector.
		/// It will not return trajectories which were created and stopped
		/// in the detector's active region.
		/// It will not return trajectories which only encounter the 
		/// non-active regions of a subdetector.
		/// The integer values specifying the detector are defined in
		/// oaAnalysis ND::ToaAnalysisUtils::ESubdetector.
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesBySubdetector(int subdetector) const;
#endif
    
		/// Fills a vector with pointers to all the trajectories, sorted
		/// in ascending ID order (for when you're doing a lot with the
		/// trajectories and you're fed up with TClonesArray).
		std::vector< ND::TTruthTrajectoriesModule::TTruthTrajectory* > GetTrajectoriesVector() const;
		
		
		
		//
		// Get Vertices
		//
		
		/// Get a vertex from its ID.
		/// If vertex is not saved, return NULL.
		ND::TTruthVerticesModule::TTruthVertex* GetVertexById(Int_t id) const;
		
		/// Get the primary vertex from which a trajectory originated.
		/// If vertex is not stored, return NULL.
		ND::TTruthVerticesModule::TTruthVertex* GetVertexOfTrajectory(Int_t trajectory_id) const;
		ND::TTruthVerticesModule::TTruthVertex* GetVertexOfTrajectory(ND::TTruthTrajectoriesModule::TTruthTrajectory* trajectory) const;
		
		/// Get the primary vertex from which a primary trajectory originated.
		/// If vertex is not stored, return NULL.
		ND::TTruthVerticesModule::TTruthVertex* GetVertexOfPrimaryTrajectory(Int_t primary_trajectory_id) const;
		ND::TTruthVerticesModule::TTruthVertex* GetVertexOfPrimaryTrajectory(ND::TTruthTrajectoriesModule::TTruthTrajectory* primary_trajectory) const;
		
		/// Get all vertices in a cuboid region defined by two
		/// TVector3s, which hold the minimum and maximum coreners of
		/// the region.
		std::vector< ND::TTruthVerticesModule::TTruthVertex* > GetVerticesByLocation(const TVector3& min_corner, const TVector3& max_corner) const;
		
		/// Get all vertices which occurred in the time window defined
		/// by min_time and max_time
		std::vector< ND::TTruthVerticesModule::TTruthVertex* > GetVerticesByTime(Double_t min_time, Double_t max_time) const;
		
		/// Get all vertices caused by the interaction of a neutrino
		/// with a given PDG code.
		std::vector< ND::TTruthVerticesModule::TTruthVertex* > GetVerticesByNeutrinoPDG(Int_t neutrino_pdg) const;
		
		/// Get all vertices in which the neutrino interacted with a
		/// target with a given PDG code.
		std::vector< ND::TTruthVerticesModule::TTruthVertex* > GetVerticesByTargetPDG(Int_t target_pdg) const;
		
		/// Get all vertices which occurred in a given subdetector.
		std::vector< ND::TTruthVerticesModule::TTruthVertex* > GetVerticesBySubdetector(int subdetector) const;
		
		/// Fills a vector with pointers to all the vertices, sorted
		/// in ascending ID order (for when you're doing a lot with the
		/// vertices and you're fed up with TClonesArray).
		std::vector< ND::TTruthVerticesModule::TTruthVertex* > GetVerticesVector() const;
		
		
	private:

                /// Private binary search method to efficiently access a 
                /// trajectory by ID.  Called by GetTrajectoryById(id).
                ND::TTruthTrajectoriesModule::TTruthTrajectory* BinaryTrajectorySearch(Int_t id, Int_t low = 0, Int_t high = -1) const;
	
                /// Private search method to efficiently access a 
                /// trajectory by ID.  Called by BinaryTrajectorySearch(id).
                ND::TTruthTrajectoriesModule::TTruthTrajectory* FindTrajectorySlowly(Int_t id) const;

	        /// Method to determine if a trajectory ClonesArray is
                /// ordered by ID.  Called by the constructor and SetTrajectoriesArray method
                void CheckTrajectoryOrdering();
	
		/// Pointer to the TClonesArray of TTruthTrajectory* into which
		/// the trajectories are being read.
		TClonesArray* fTrajectories;
		
		/// Pointer to the TClonesArray of TTruthVertices* into which
		/// the vertices being read.
		TClonesArray* fVertices;
		
		/// Pointer to the Int_t into which the number of trajectories
		/// in the event is being read.
		Int_t* fNTrajectories;
		
		/// Pointer to the Int_t into which the number of vertices in
		/// the event is being read.
		Int_t* fNVertices;

  //Vebosity controller for the TTruthUtils Error messages
  Int_t verbosity;

                /// Bool to indicate whether the trajectory TClonesArray is 
                /// ordered according to ID or not.
                bool fOrdered;

};

#endif

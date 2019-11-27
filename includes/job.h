#pragma once
#include <string>
#include <chrono>
#include <vector>

namespace ClusterSimulator
{
	enum class JobState
	{
		PEND, RUN, DONE, EXIT, PSUSP, USUSP, SSUSP, POST_DONE, POST_ERR, UNKWN, WAIT, ZOMBI
	};
	
	struct ScenarioEntry;
	class Queue;
	class Host;
	using namespace std::chrono;
	using ms = time_point<milliseconds>;
	class Job
	{
	public:
		/* Job fields from a scenario entry*/

		int id{ id_gen_++ };
		int slot_required;			// The actual number of slots used for job execution.
		long mem_required;
		ms submit_time;
		milliseconds cpu_time;
		milliseconds non_cpu_time;		
		Queue* queue_managing_this_job;

		milliseconds run_time;		// Estimated run time: cpu_time / factor + non_cpu_time
		
		/* Disabled fields */
		// bool is_multi_host;
		// long mem_usage;
		// long swap_usage;
		//double num_exec_procs;	// The number of processors that the job initially requested for execution.

		ms start_time{};
		ms finish_time{};
		milliseconds total_pending_duration{};

		int priority{ 0 };
		JobState state{ JobState::WAIT };

		Job(const ScenarioEntry& entry, Queue& queue, const ms submit_time);

		// const std::string& get_application_name() const { return application_name_; }
		// const std::string& get_dedicated_host_name() const { return dedicated_host_name_; }
		// const std::string& get_exit_host_status() const { return exit_host_status_; }
		const std::string& get_run_host_name() const { return *run_host_name_; }
		void set_run_host_name(const std::string& run_host_name) { run_host_name_ = &run_host_name; }
		void set_pending(ms time) noexcept
		{
			state = JobState::PEND;
			if (pend_start_time_ == ms{})
				pend_start_time_ = time;
		}
		void update_total_pending_duration(ms current_time) 
		{ 
			total_pending_duration = current_time - pend_start_time_; 
		}

		/**
		 * Gets the list of Hosts eligible to run this job.
		 */
		std::vector<Host*> get_eligible_hosts() const;

	private:
		// std::string application_name_;
		// std::string dedicated_host_name_;
		// std::string exit_host_status_;
		const std::string* run_host_name_;

		ms pend_start_time_{};

		//unsigned long id_;
		//double submitted_time_;
		//int num_cpu_required_;
		//int task_index_;
		//std::vector<Task> tasks_;

		static int id_gen_;
	};
}



use std::{env, fs};

/* FCFS */
#[derive(Debug)]
struct SimulationFCFS {
    current_time: i32,
    current_process: Vec<i32>,
    process_queue: Vec<Vec<i32>>, // Order Matters
    process_finished: Vec<Vec<i32>>,
}

impl SimulationFCFS {
    pub fn execute_task(&mut self) {
        // Process finished
        if self.current_process[5] == self.current_process[2] {
            // Calc running time
            self.current_process[4] += self.current_time;

            // Calc waiting time
            self.current_process[6] = self.current_process[4] - self.current_process[1] - self.current_process[2];

            println!("PID\tArrival Time\tStart Time\tEnd Time\tRunning Time\tWaiting Time\n{}\t{}\t\t{}\t\t{}\t\t{}\t\t{}\n", self.current_process[0], self.current_process[1], self.current_process[3], self.current_process[4], self.current_process[5], self.current_process[6]);

            // Get new process
            let proc = self.process_queue.pop();
            self.process_finished.push(self.current_process.clone());
            if proc.is_none() {
                self.current_process = Vec::new();
                return
            }

            // Set new rpocess
            self.current_process = proc.unwrap();
            
            // Set start time
            self.current_process[3] = self.current_time;
        }

        // Continue executing
        self.current_process[5] += 1;
        self.current_time += 1;
    }

    pub fn queue_task(&mut self, task: Vec<i32>) {
        self.process_queue.insert(0, task);
    }
}

#[derive(Debug)]
struct SimulationSJF {
    current_time: i32,
    current_process: Vec<i32>,
    process_queue: Vec<Vec<i32>>,
    process_finished: Vec<Vec<i32>>,
}

impl Default for SimulationSJF {
    fn default() -> Self {
        Self {
            current_time: 0,
            current_process: Vec::new(),
            process_queue: Vec::new(),
            process_finished: Vec::new(),
        }
    }
}

impl SimulationSJF {
    pub fn execute(&mut self) {
        // If both are empty, return
        if self.process_queue.is_empty() && self.current_process.is_empty() { return };

        // If process is not set, set it
        if self.current_process.is_empty() {
            // Find next shortest process
            let process = {
                let mut shortest_index = 0;
                for i in 0..self.process_queue.len() {
                    if self.process_queue[i][2] < self.process_queue[shortest_index][2] {
                        shortest_index = i;
                    }
                }

                self.process_queue.remove(shortest_index)
            };
            
            // Set it
            self.current_process = process;

            // Set start time
            self.current_process[3] = self.current_time;
        }

        // Execute
        self.current_process[5] += 1;
        self.current_time += 1;

        // Process has finished
        if self.current_process[5] == self.current_process[2] {
            // Set current time
            self.current_process[4] += self.current_time;

            // Set Waiting Time
            self.current_process[6] = self.current_process[4] - self.current_process[1] - self.current_process[2];

            println!("PID\tArrival Time\tStart Time\tEnd Time\tRunning Time\tWaiting Time\n{}\t{}\t\t{}\t\t{}\t\t{}\t\t{}\n", self.current_process[0], self.current_process[1], self.current_process[3], self.current_process[4], self.current_process[5], self.current_process[6]);
            
            // Add to finished processes
            self.process_finished.push(self.current_process.clone());
            
            // Reset current process
            self.current_process = Vec::new();
        }
    }

    pub fn queue_task(&mut self, task: Vec<i32>) {
        self.process_queue.insert(0, task);
    }
}

#[derive(Debug)]
struct SimulationRR {
    quantum: i32,
    current_time: i32,
    current_process: Vec<i32>,
    process_queue: Vec<Vec<i32>>,
    process_finished: Vec<Vec<i32>>,
}

impl Default for SimulationRR {
    fn default() -> Self {
        Self {
            quantum: 0,
            current_time: 0,
            current_process: Vec::new(),
            process_queue: Vec::new(),
            process_finished: Vec::new(),
        }
    }
}

impl SimulationRR {
    pub fn execute(&mut self) {
        // If process queue and process are empty, return
        if self.process_queue.is_empty() && self.current_process.is_empty() { return };

        // Queue new process if no process is set
        if self.current_process.is_empty() {
            // Get the next process and set it
            let process = self.process_queue.pop().unwrap();
            self.current_process = process;

            // Set current process' time
            self.current_process[3] = self.current_time;
        }

        // Execute
        self.current_process[5] += 1;
        self.current_time += 1;

        // Remove process if finished executing
        if self.current_process[5] == self.current_process[2] {
            // Set end time
            self.current_process[4] = self.current_time;

            // Set waiting time
            self.current_process[6] = self.current_process[4] - self.current_process[1] - self.current_process[2];

            println!("PID\tArrival Time\tStart Time\tEnd Time\tRunning Time\tWaiting Time\n{}\t{}\t\t{}\t\t{}\t\t{}\t\t{}\n", self.current_process[0], self.current_process[1], self.current_process[3], self.current_process[4], self.current_process[5], self.current_process[6]);
            
            // Add to finished processes
            self.process_finished.push(self.current_process.clone());

            // Reset process
            self.current_process = Vec::new();
        }

        // Move to next process if quantum is reached
        if self.current_time % self.quantum == 0 && !self.current_process.is_empty() {
            
            // Time limit has been reached
            self.process_queue.insert(0, self.current_process.clone());
            self.current_process = self.process_queue.pop().unwrap();
        }

    }

    pub fn queue_task(&mut self, task: Vec<i32>) {
        self.process_queue.insert(0, task);
    }
}

fn getInput(file: String) -> Vec<Vec<i32>> {
    let mut contents = fs::read_to_string(file).expect("Should be able to read this");
    contents = contents.replace('\n', " ");
    let mut numsStr: Vec<&str> = contents.split(' ').collect();
    numsStr.pop();

    let mut nums = Vec::new();
    for num in numsStr {
        nums.push(num.parse::<i32>().unwrap());
    }

    nums.reverse();

    let mut ret: Vec<Vec<i32>> = Vec::new();
    nums.pop();

    for _ in (0..nums.len()).step_by(3) {
        let mut v = Vec::new();
        v.push(nums.pop().unwrap());
        v.push(nums.pop().unwrap());
        v.push(nums.pop().unwrap());

        for i in 0..4 {
            v.push(0);
        }

        ret.push(v);
    }

    return ret;
}

fn main() {
    let args: Vec<String> = env::args().collect();

    let path = args[1].clone();

    let vec = getInput(path);

    match args[2].clone().as_str() {
        "FCFS" => {
            let mut fcfs_vec = vec.clone();
            println!("Average Waiting Time: {}\n", FCFS(&mut fcfs_vec));
        }

        "SJF" => {
            let mut sjf_vec = vec.clone();
            println!("Average Waiting Time: {}\n", SJF(&mut sjf_vec));
        }

        "RR" => {
            let quantum = args[3].parse::<i32>().unwrap();
            let mut rr_vec = vec.clone();
            println!("Average Waiting Time: {}\n", RR(&mut rr_vec, quantum));
        }

        _ => {}
    }
}

fn FCFS(processes: &mut Vec<Vec<i32>>) -> f64 {
    // [pid, arrival_time, burst time, start time, end time, running time, waiting time]
    // pid arrival burst already filled    
    let mut sim = SimulationFCFS { current_time: 0, current_process: processes[0].clone(), process_queue: Vec::new(), process_finished: Vec::new() };

    for i in 1..processes.len() {
        sim.queue_task(processes[i].clone());
    }

    for _ in 0..20 {
        sim.execute_task();
    }

    *processes = sim.process_finished.clone();

    let mut total = 0;
    for i in 0..processes.len() {
        total += processes[i][6];
    }
    return total as f64 / processes.len() as f64;
 
}

fn SJF(processes: &mut Vec<Vec<i32>>) -> f64 {
    // [pid, arrival_time, burst time, start time, end time, running time, waiting time]
    let mut sjf = SimulationSJF::default();
    for i in 0..20 {
        for process in &mut *processes {
            if process[1] as usize == i {
                sjf.queue_task(process.clone());
            }
        }

        sjf.execute();
    }

    *processes = sjf.process_finished.clone(); 

    let mut total = 0;
    for i in 0..processes.len() {
        total += processes[i][6];
    }

    return total as f64 / processes.len() as f64;
}

fn RR(processes: &mut Vec<Vec<i32>>, quantum: i32) -> f64 {
    // [pid, arrival_time, burst time, start time, end time, running time, waiting time]
    let mut rr = SimulationRR::default();
    rr.quantum = quantum;

    for i in 0..20 {
        for process in &mut *processes {
            if process[1] as usize == i {
                rr.queue_task(process.clone());
            }
        }

        rr.execute();
    }

    *processes = rr.process_finished.clone(); 

    let mut total = 0;
    for i in 0..processes.len() {
        total += processes[i][6];
    }

    return total as f64 / processes.len() as f64;
}

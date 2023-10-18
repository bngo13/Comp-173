#[derive(Debug)]
struct SimulationFCFS {
    current_time: i32,
    current_process: Vec<i32>,
    process_queue: Vec<Vec<i32>>, // Order Matters
    process_finished: Vec<Vec<i32>>,
}

impl SimulationFCFS {
    pub fn execute_task(&mut self) {
        if self.current_process[5] == self.current_process[2] {
            // Process finished
           
            // Calc running time
            self.current_process[4] += self.current_time;

            // Calc waiting time
            self.current_process[6] = self.current_process[4] - self.current_process[1] - self.current_process[2];


            // Get new process
            let proc = self.process_queue.pop();
            self.process_finished.push(self.current_process.clone());
            if proc.is_none() {
                self.current_process = Vec::new();
                return
            }
            self.current_process = proc.unwrap();
            self.current_process[3] = self.current_time;
        }


        self.current_process[5] += 1;

        self.current_time += 1;
    }

    pub fn queue_task(&mut self, task: Vec<i32>) {
        self.process_queue.insert(0, task);
    }
}

fn main() {
    let vec = vec![
        vec![0, 0, 12, 0, 0, 0, 0], 
        vec![1, 2, 4, 0, 0, 0, 0],
        vec![2, 3, 1, 0, 0, 0, 0],
        vec![3, 4, 2, 0, 0, 0, 0],
    ];

    let mut fcfs_vec = vec.clone();
    println!("{}", FCFS(&mut fcfs_vec));
    println!("{:?}", fcfs_vec);

    let mut sjf_vec = vec.clone();
}

fn FCFS(processes: &mut Vec<Vec<i32>>) -> i32 {
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
    return total / processes.len() as i32;
 
}

fn SJF(processes: &mut Vec<Vec<i32>>) {
    // [pid, arrival_time, burst time, start time, end time, running time, waiting time]
}

fn RR(processes: &mut Vec<Vec<i32>>, quantum: i32) {
    // [pid, arrival_time, burst time, start time, end time, running time, waiting time]
}

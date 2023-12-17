use colored::*;
use regex::Regex;
use std::{
    collections::HashMap,
    fmt::Display,
    fs, time::Instant,
};

const DAY: u32 = 8;

fn run<F, Res>(name: &str, f: F, input: &str)
where
    F: FnOnce(&str) -> Res,
    Res: Display,
{
    let t = Instant::now();
    let result = f(input);
    let d = t.elapsed();
    let formatted_name = if name.contains("sample") {
        format!("{:14}: ", name).cyan()
    } else {
        format!("{:14}: ", name).yellow()
    };

    println!(
        "{}{} {}",
        formatted_name,
        format!("{:10}", result).bold(),
        format!("({:.3} ms)", d.as_secs_f64() * 1000.).bright_black()
    );
}

fn main() {
    let sample = fs::read_to_string(format!("samples/day{:02}", DAY)).unwrap_or_default();
    let input = fs::read_to_string(format!("inputs/day{:02}", DAY)).unwrap_or_default();

    run("Part 1 sample", part1, &sample);
    run("Part 1 input", part1, &input);
    run("Part 2 sample", part2, &sample);
    run("Part 2 input", part2, &input);
}

fn part1(input: &str) -> u64 {
    let mut lines = input.lines();
    let instructions = lines.next().unwrap();
    lines.next();

    let mut nodes = HashMap::new();

    let re = Regex::new(r"(\w+) = \((\w+), (\w+)\)").unwrap();
    for line in lines {
        let captures = re.captures(line).unwrap();
        let node = captures.get(1).unwrap().as_str();
        let left = captures.get(2).unwrap().as_str();
        let right = captures.get(3).unwrap().as_str();
        nodes.insert(node, (left, right));
    }

    let mut current = "AAA";
    let mut steps = 0;
    for instruction in instructions.chars().cycle() {
        steps += 1;
        current = match instruction {
            'L' => nodes.get(current).unwrap().0,
            'R' => nodes.get(current).unwrap().1,
            _ => panic!("not a valid instruction: {}", instruction),
        };
        if current == "ZZZ" {
            break;
        }
    }
    steps
}

fn part2(input: &str) -> u64 {
    let mut lines = input.lines();
    let instructions = lines.next().unwrap();
    lines.next();

    let mut nodes = HashMap::new();

    let re = Regex::new(r"(\w+) = \((\w+), (\w+)\)").unwrap();
    for line in lines {
        let captures = re.captures(line).unwrap();
        let node = captures.get(1).unwrap().as_str();
        let left = captures.get(2).unwrap().as_str();
        let right = captures.get(3).unwrap().as_str();
        nodes.insert(node, (left, right));
    }

    let mut ends = vec![];

    for start in nodes.keys().copied().filter(|s| s.ends_with("A")) {
        let mut current = start;
        let mut step = 0;

        for instruction in instructions.chars().cycle() {
            step += 1;
            current = match instruction {
                'L' => nodes.get(current).unwrap().0,
                'R' => nodes.get(current).unwrap().1,
                _ => panic!("not a valid instruction: {}", instruction),
            };
            if current.ends_with("Z") {
                ends.push(step);
                break;
            }
        }
    }

    dbg!(&ends);
    lcm(&ends)
}

fn lcm(v: &[u64]) -> u64 {
    let max = *v.iter().max().unwrap();
    let mut res = max;
    while !v.iter().copied().all(|a| res % a == 0) {
        res += max;
    }
    res
}

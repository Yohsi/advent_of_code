use colored::*;
use std::{fmt::Display, fs, time::Instant};

const DAY: u32 = 6;

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
    let times: Vec<u64> = lines
        .next()
        .unwrap()
        .strip_prefix("Time:")
        .unwrap()
        .trim()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();
    let distances: Vec<u64> = lines
        .next()
        .unwrap()
        .strip_prefix("Distance:")
        .unwrap()
        .trim()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let mut margin_of_error = 1;

    for (time, record_distance) in times.iter().copied().zip(distances) {
        // time = speed + distance / speed
        // distance = time*speed - speed^2

        // record_distance < distance
        // record_distance < time*speed - speed^2
        // speed^2 - time*speed + record_distance < 0
        let a = 1.0;
        let b = -(time as f64);
        let c = record_distance as f64;
        let sqrt_delta = (b * b - 4.0 * a * c).sqrt();
        let speed1 = (-b - sqrt_delta) / (2.0 * a);
        let speed2 = (-b + sqrt_delta) / (2.0 * a);
        let speed_min = speed1.floor() as u64 + 1;
        let speed_max = speed2.ceil() as u64 - 1;
        let nb_ways_to_win = speed_max - speed_min + 1;
        margin_of_error *= nb_ways_to_win;
    }
    margin_of_error
}


fn part2(input: &str) -> u64 {
    let mut lines = input.lines();
    let time: u64 = lines
        .next()
        .unwrap()
        .strip_prefix("Time:")
        .unwrap()
        .replace(" ", "")
        .parse()
        .unwrap();
    let record_distance: u64 = lines
        .next()
        .unwrap()
        .strip_prefix("Distance:")
        .unwrap()
        .replace(" ", "")
        .parse()
        .unwrap();

    let a = 1.0;
    let b = -(time as f64);
    let c = record_distance as f64;
    let sqrt_delta = (b * b - 4.0 * a * c).sqrt();
    let speed1 = (-b - sqrt_delta) / (2.0 * a);
    let speed2 = (-b + sqrt_delta) / (2.0 * a);
    let speed_min = speed1.floor() as u64 + 1;
    let speed_max = speed2.ceil() as u64 - 1;
    let nb_ways_to_win = speed_max - speed_min + 1;
    nb_ways_to_win
}


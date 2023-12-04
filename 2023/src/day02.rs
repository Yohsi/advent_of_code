use colored::*;
use std::{cmp::max, collections::HashMap, fmt::Display, fs, time::Instant};

const DAY: u32 = 2;

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
        format!("{:5}", result).bold(),
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

fn part1(input: &str) -> usize {
    let max_by_color = HashMap::from([
        ("red".to_owned(), 12),
        ("green".to_owned(), 13),
        ("blue".to_owned(), 14),
    ]);

    input
        .lines()
        .map(|l| {
            l.split_once(": ")
                .unwrap()
                .1
                .split(";")
                .map(|sample| {
                    sample
                        .split(",")
                        .map(|cubes| {
                            let (nb, color) = cubes.trim().split_once(" ").unwrap();
                            (color.to_owned(), nb.parse().unwrap())
                        })
                        .collect()
                })
                .collect::<Vec<HashMap<String, u64>>>()
        })
        .enumerate()
        .filter(|(_, game)| {
            game.iter().all(|sample| {
                sample
                    .iter()
                    .all(|(color, nb)| nb <= max_by_color.get(color).unwrap())
            })
        })
        .map(|(index, _)| index + 1)
        .sum()
}

fn part2(input: &str) -> u64 {
    input
        .lines()
        .map(|l| {
            l.split_once(": ")
                .unwrap()
                .1
                .split(";")
                .map(|sample| {
                    sample
                        .split(",")
                        .map(|cubes| {
                            let (nb, color) = cubes.trim().split_once(" ").unwrap();
                            (color.to_owned(), nb.parse().unwrap())
                        })
                        .collect()
                })
                .collect::<Vec<HashMap<String, u64>>>()
        })
        .map(|sample| {
            sample
                .into_iter()
                .fold([0, 0, 0], |current_max, map| {
                    [
                        max(current_max[0], map.get("red").cloned().unwrap_or(0)),
                        max(current_max[1], map.get("blue").cloned().unwrap_or(0)),
                        max(current_max[2], map.get("green").cloned().unwrap_or(0)),
                    ]
                })
                .iter()
                .product::<u64>()
        })
        .sum()
}

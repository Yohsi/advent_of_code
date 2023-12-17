use colored::*;
use std::{collections::HashSet, fmt::Display, fs, time::Instant};

const DAY: u32 = 4;

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

fn part1(input: &str) -> u64 {
    let mut points = 0;

    for line in input.lines() {
        let (_, number_line) = line.split_once(':').unwrap();
        let (winning, numbers) = number_line.split_once('|').unwrap();

        let winning: HashSet<u32> = winning
            .trim()
            .split_whitespace()
            .map(|s| s.parse().unwrap())
            .collect();

        let numbers: HashSet<u32> = numbers
            .trim()
            .split_whitespace()
            .map(|s| s.parse().unwrap())
            .collect();

        let nb_same = winning.intersection(&numbers).count() as u32;
        let card_points = if nb_same == 0 {
            0
        } else {
            2u64.pow(nb_same - 1)
        };
        points += card_points;
    }
    points
}

fn part2(input: &str) -> u64 {
    let mut total = 0;

    let mut copies = vec![];
    let mut card_nb = 0;

    for line in input.lines() {
        card_nb += 1;
        let (_, number_line) = line.split_once(':').unwrap();
        let (winning, numbers) = number_line.split_once('|').unwrap();

        let winning: HashSet<u32> = winning
            .trim()
            .split_whitespace()
            .map(|s| s.parse().unwrap())
            .collect();

        let numbers: HashSet<u32> = numbers
            .trim()
            .split_whitespace()
            .map(|s| s.parse().unwrap())
            .collect();

        let nb_of_this_card = 1 + copies
            .iter()
            .filter(|(until, _)| *until >= card_nb)
            .map(|(_, nb)| nb)
            .sum::<u64>();
        let nb_same = winning.intersection(&numbers).count() as u32;

        if nb_same > 0 {
            copies.push((card_nb + nb_same, nb_of_this_card));
        }
        total += nb_of_this_card;
    }
    total
}

use colored::*;
use regex::Regex;
use std::{
    cmp::{max, min},
    collections::HashMap,
    fmt::Display,
    fs,
    time::Instant,
};

const DAY: u32 = 5;

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

struct Range {
    pub src_start: u64,
    pub dest_start: u64,
    pub len: u64,
}

impl Range {
    pub fn parse(line: &str) -> Range {
        let split: Vec<u64> = line
            .split_whitespace()
            .map(|s| s.parse().unwrap())
            .collect();
        Range {
            src_start: split[1],
            dest_start: split[0],
            len: split[2],
        }
    }

    pub fn map(&self, a: u64) -> Option<u64> {
        if a < self.src_start || a >= self.src_start + self.len {
            return None;
        }
        Some(self.dest_start + (a - self.src_start))
    }

    pub fn map_range(&self, a: &NumberRange) -> NumberRangeMapping {
        if a.end <= self.src_start || a.start >= self.src_start + self.len {
            return NumberRangeMapping {
                mapped: None,
                remaining: vec![*a],
            };
        }
        let actual_start = max(self.src_start, a.start);
        let actual_end = min(self.src_start + self.len, a.end);

        let mut remaining = vec![];

        if a.start < actual_start {
            remaining.push(NumberRange {
                start: a.start,
                end: actual_start,
            })
        }
        if actual_end < a.end {
            remaining.push(NumberRange {
                start: actual_end,
                end: a.end,
            })
        }

        NumberRangeMapping {
            mapped: Some(NumberRange {
                start: actual_start + self.dest_start - self.src_start,
                end: actual_end + self.dest_start - self.src_start,
            }),
            remaining,
        }
    }
}

struct Mapping {
    pub src: String,
    pub dest: String,
    pub ranges: Vec<Range>,
}

impl Mapping {
    pub fn parse(lines: &[&str]) -> Mapping {
        let header = lines[0];
        let re = Regex::new(r"(\w+)-to-(\w+) map:").unwrap();
        let captures = re.captures(header).unwrap();
        let src = captures.get(1).unwrap().as_str().parse().unwrap();
        let dest = captures.get(2).unwrap().as_str().parse().unwrap();
        let mut ranges = vec![];
        for line in &lines[1..] {
            ranges.push(Range::parse(line));
        }
        Mapping { src, dest, ranges }
    }

    pub fn map(&self, a: u64) -> u64 {
        for r in &self.ranges {
            match r.map(a) {
                Some(b) => return b,
                None => (),
            }
        }
        a
    }
    pub fn map_range(&self, a: &NumberRange) -> Vec<NumberRange> {
        let mut ranges = vec![];
        let mut remaining = vec![*a];

        for r in &self.ranges {
            let mut new_remaining = vec![];
            for rem in remaining {
                let mut range_mapping = r.map_range(&rem);
                match range_mapping.mapped {
                    Some(b) => ranges.push(b),
                    None => (),
                }
                new_remaining.append(&mut range_mapping.remaining);
            }
            remaining = new_remaining;
        }
        ranges.append(&mut remaining);
        ranges
    }
}

fn part1(input: &str) -> u64 {
    let mut lines = input.lines();
    let seeds: Vec<u64> = lines.next().unwrap()[7..]
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let mut mapping_str_list = vec![];

    for line in lines {
        if line.is_empty() {
            mapping_str_list.push(vec![]);
            continue;
        }
        mapping_str_list.last_mut().unwrap().push(line);
    }

    let mut mappings = HashMap::new();

    for mapping_str in mapping_str_list {
        let mapping = Mapping::parse(&mapping_str);
        mappings.insert(mapping.src.clone(), mapping);
    }

    seeds
        .iter()
        .map(|seed| {
            let mut number = *seed;
            let mut current_type = "seed";
            while current_type != "location" {
                let mapping = mappings.get(current_type).unwrap();
                number = mapping.map(number);
                current_type = &mapping.dest;
            }
            number
        })
        .min()
        .unwrap()
}

#[derive(Clone, Copy)]
struct NumberRange {
    start: u64,
    end: u64,
}

struct NumberRangeMapping {
    mapped: Option<NumberRange>,
    remaining: Vec<NumberRange>,
}

fn part2(input: &str) -> u64 {
    let mut lines = input.lines();
    let raw_seeds: Vec<u64> = lines.next().unwrap()[7..]
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    let mut seed_ranges: Vec<_> = raw_seeds
        .chunks(2)
        .map(|chunk| NumberRange {
            start: chunk[0],
            end: chunk[0] + chunk[1],
        })
        .collect();

    let mut mapping_str_list = vec![];

    for line in lines {
        if line.is_empty() {
            mapping_str_list.push(vec![]);
            continue;
        }
        mapping_str_list.last_mut().unwrap().push(line);
    }

    let mut mappings = HashMap::new();

    for mapping_str in mapping_str_list {
        let mapping = Mapping::parse(&mapping_str);
        mappings.insert(mapping.src.clone(), mapping);
    }

    let mut current_type = "seed";
    while current_type != "location" {
        let mapping = mappings.get(current_type).unwrap();
        let mut new_ranges = vec![];
        for range in &seed_ranges {
            new_ranges.append(&mut mapping.map_range(range));
        }
        current_type = &mapping.dest;
        seed_ranges = new_ranges;
    }

    seed_ranges.iter().map(|range| range.start).min().unwrap()
}

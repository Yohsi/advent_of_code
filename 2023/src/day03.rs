use colored::*;
use std::{fmt::Display, fs, ops::Range, time::Instant};

const DAY: u32 = 3;

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
    let map: Vec<Vec<char>> = input.lines().map(|l| l.chars().collect()).collect();

    let mut current_number: Option<u32> = None;
    let mut include_current = false;
    let mut sum: u64 = 0;

    for y in 0..map.len() {
        for x in 0..map[y].len() {
            let c = map[y][x];
            match c {
                '0'..='9' => {
                    let d = c.to_digit(10).unwrap();

                    match current_number {
                        None => current_number = Some(d),
                        Some(n) => current_number = Some(n * 10 + d),
                    }
                    include_current |= is_adjacent_to_symbol(&map, x, y);
                }
                _ => {
                    if include_current {
                        if let Some(n) = current_number {
                            sum += n as u64;
                        }
                    }
                    current_number = None;
                    include_current = false;
                }
            }
        }
        if include_current {
            if let Some(n) = current_number {
                sum += n as u64;
            }
        }
        current_number = None;
        include_current = false;
    }
    sum
}

fn is_adjacent_to_symbol(map: &[Vec<char>], x: usize, y: usize) -> bool {
    for dx in x.saturating_sub(1)..=x + 1 {
        for dy in y.saturating_sub(1)..=y + 1 {
            if dx == x && dy == y {
                continue;
            }
            if map
                .get(dy)
                .and_then(|line| line.get(dx))
                .is_some_and(|c| !c.is_digit(10) && *c != '.')
            {
                return true;
            }
        }
    }
    return false;
}

fn part2(input: &str) -> u64 {
    let map: Vec<Vec<char>> = input.lines().map(|l| l.chars().collect()).collect();

    let mut sum: u64 = 0;

    for y in 0..map.len() {
        for x in 0..map[y].len() {
            let c = map[y][x];
            if c == '*' {
                sum += compute_gear_ratio(&map, x, y).unwrap_or_default() as u64;
            }
        }
    }
    sum
}

// compute the gear ratio of a '*' if there are two adjacent gears
fn compute_gear_ratio(map: &[Vec<char>], x: usize, y: usize) -> Option<u32> {
    let mut gear1: Option<u32> = None;
    let mut gear1_range_x: Option<Range<usize>> = None;
    let mut gear1_y = None;

    for dx in x.saturating_sub(1)..=x + 1 {
        for dy in y.saturating_sub(1)..=y + 1 {
            // skip the * itself
            if dx == x && dy == y {
                continue;
            }

            // skip remaining digits of the first number
            if gear1_y == Some(dy)
                && gear1_range_x
                    .as_ref()
                    .is_some_and(|range| range.contains(&dx))
            {
                continue;
            }

            if let Some((gear, range)) = parse_number(map, dx, dy) {
                if let Some(gear1) = gear1 {
                    return Some(gear1 * gear);
                } else {
                    gear1 = Some(gear);
                    gear1_range_x = Some(range);
                    gear1_y = Some(dy);
                }
            }
        }
    }
    return None;
}

// if there is a number at that position, returns this number 
// and the range of x coords containing the number
fn parse_number(map: &[Vec<char>], x: usize, y: usize) -> Option<(u32, Range<usize>)> {
    let mut mx = x as isize;

    // go to the beginning of the number
    while map
        .get(y)
        .and_then(|line| line.get(mx as usize))
        .is_some_and(|c| c.is_digit(10))
    {
        mx -= 1;
        if mx < 0 {
            break;
        }
    }

    // we did not move -> this is not a number
    if mx == x as isize {
        return None;
    }

    // add all digits until the end
    let mut x = (mx + 1) as usize;
    let xstart = x;
    let mut nb = 0;
    while map
        .get(y)
        .and_then(|line| line.get(x))
        .is_some_and(|c| c.is_digit(10))
    {
        nb = nb * 10 + map[y][x].to_digit(10).unwrap();
        x += 1;
        if x >= map[y].len() {
            break;
        }
    }
    Some((nb, xstart..x))
}

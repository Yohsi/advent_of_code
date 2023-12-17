use colored::*;
use enum_map::{Enum, EnumMap};
use std::{fmt::Display, fs, time::Instant};

const DAY: u32 = 7;

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

#[derive(PartialEq, Eq, PartialOrd, Ord, Clone, Copy, Debug)]
enum HandType {
    HighCard,
    Pair,
    TwoPairs,
    Three,
    Full,
    Four,
    Five,
}

impl HandType {
    fn compute(cards: &[Card]) -> Self {
        let mut nb_cards: EnumMap<Card, u8> = EnumMap::default();
        for card in cards {
            nb_cards[*card] += 1;
        }

        // for part 2: replace jokers and count again
        let max_card = nb_cards
            .iter()
            .filter(|(c, _)| *c != Card::Joker)
            .max_by_key(|(_, n)| *n)
            .unwrap()
            .0;
        let cards = cards
            .iter()
            .map(|c| if *c == Card::Joker { max_card } else { *c });
        let mut nb_cards: EnumMap<Card, u8> = EnumMap::default();
        for card in cards {
            nb_cards[card] += 1;
        }

        let values = nb_cards.values().copied();
        if values.clone().any(|n| n == 5) {
            return Self::Five;
        }
        if values.clone().any(|n| n == 4) {
            return Self::Four;
        }
        let contains_three_of_a_kind = values.clone().any(|n| n == 3);
        let nb_pairs = values.clone().filter(|n| *n == 2).count();

        if contains_three_of_a_kind && nb_pairs == 1 {
            return Self::Full;
        }
        if contains_three_of_a_kind {
            return Self::Three;
        }
        if nb_pairs == 2 {
            return Self::TwoPairs;
        }
        if nb_pairs == 1 {
            return Self::Pair;
        }
        return HandType::HighCard;
    }
}

#[derive(PartialEq, Eq, PartialOrd, Ord, Clone, Copy, Enum, Debug)]
enum Card {
    Joker,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    T,
    J,
    Q,
    K,
    A,
}

impl Card {
    fn parse(c: char, parse_jokers: bool) -> Self {
        match c {
            '2' => Self::_2,
            '3' => Self::_3,
            '4' => Self::_4,
            '5' => Self::_5,
            '6' => Self::_6,
            '7' => Self::_7,
            '8' => Self::_8,
            '9' => Self::_9,
            'T' => Self::T,
            'J' => {
                if parse_jokers {
                    Self::Joker
                } else {
                    Self::J
                }
            }
            'Q' => Self::Q,
            'K' => Self::K,
            'A' => Self::A,
            _ => panic!("unexpected character: {}", c),
        }
    }
}

#[derive(PartialEq, Eq, PartialOrd, Ord, Clone, Copy, Debug)]
struct Hand {
    hand_type: HandType,
    cards: [Card; 5],
}

impl Hand {
    fn parse(s: &str, parse_jokers: bool) -> Hand {
        let mut chars = s.chars();
        let cards = [
            Card::parse(chars.next().unwrap(), parse_jokers),
            Card::parse(chars.next().unwrap(), parse_jokers),
            Card::parse(chars.next().unwrap(), parse_jokers),
            Card::parse(chars.next().unwrap(), parse_jokers),
            Card::parse(chars.next().unwrap(), parse_jokers),
        ];

        let hand_type = HandType::compute(&cards);

        Hand { cards, hand_type }
    }
}

fn part1(input: &str) -> u64 {
    let mut hands: Vec<(Hand, u64)> = input
        .lines()
        .map(|line| line.split_once(" ").unwrap())
        .map(|(cards_str, bid_str)| (Hand::parse(cards_str, false), bid_str.parse().unwrap()))
        .collect();
    hands.sort_unstable_by_key(|(h, _)| *h);

    hands
        .iter()
        .map(|(_, b)| b)
        .enumerate()
        .map(|(i, bid)| (i as u64 + 1) * bid)
        .sum()
}

fn part2(input: &str) -> u64 {
    let mut hands: Vec<(Hand, u64)> = input
        .lines()
        .map(|line| line.split_once(" ").unwrap())
        .map(|(cards_str, bid_str)| (Hand::parse(cards_str, true), bid_str.parse().unwrap()))
        .collect();
    hands.sort_unstable_by_key(|(h, _)| *h);

    hands
        .iter()
        .map(|(_, b)| b)
        .enumerate()
        .map(|(i, bid)| (i as u64 + 1) * bid)
        .sum()
}

import re;

def format_bubbles(value):
    units = [
        (10**12, 'T'),
        (10**9, 'B'),
        (10**6, 'M'),
        (10**3, 'K')
    ];

    for threshold, suffix in units:
        if value >= threshold:
            formatted = value / threshold;
            return f"{formatted:.1f}".rstrip('0').rstrip('.') + suffix;
    return str(int(value));

def format_patch_note(line):
    match = re.match(r'"\s*(.*?)\s*"\s*,\s*([\d.]+)\s*,\s*([\d.]+)', line);
    if not match:
        return "Invalid line format";

    name = match.group(1);
    unlock_cost = float(match.group(2));
    current_cost = float(match.group(3));

    unlock_str = format_bubbles(unlock_cost);
    current_str = format_bubbles(current_cost);

    return f"+ {name} | {unlock_str} Bubble Unlock\n> {current_str} Bubbles";

lines = [   # Input goes here (ie. '"Blue Soap Bar", 12000000.0, 14000000.0')
    '"Large Loofah", 27500000.0, 32500000.0'
];

for line in lines:
    print(format_patch_note(line) + "\n");
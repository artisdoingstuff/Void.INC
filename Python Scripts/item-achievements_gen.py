milestones = [
    1, 10, 25, 50, 75, 100, 150, 200, 250,
    #300, 400, 500, 600, 700, 800, 900, 1000
];

item = [
    "Hand Wash", "Shampoo", "Shaving Foam", "Toothpaste",
    "Loofah", "Bubble Bath", "Bathtub Jet", "Luxury Spa",
    "Foam Pit", "Foam Party", "Sudsy Soap", "Bubble Machine",
    "Bubbly Pool", "Sparkling Water", "Carbonated Soda", "Bath Bombs",
    "Bubble Wand"
];

item_counter = 0;
counter = 0;

for i in item:
    print(f"\n// {item[item_counter]} Achievements");
    for j in milestones:
        print('achievements.push_back({ "", ' + f'"Purchase {milestones[counter]} {item[item_counter]}", AchievementType::SpecificUpgrade, {milestones[counter]}, "{item[item_counter]}"' +  '});');
        counter += 1;
    item_counter += 1;
    counter -= counter;
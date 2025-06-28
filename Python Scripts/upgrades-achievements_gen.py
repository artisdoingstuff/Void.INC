milestones = [
    1, 10, 25, 50, 75, 100, 150, 200, 250,
    300, 400, 500, 600, 700, 800, 900, 1000,
    1100, 1200, 1300, 1400, 1500, 1750, 2000,
    2250, 2500, 3000, 3500, 4000, 5000,
    6000, 7000, 8000, 9000, 10000
];

counter = 0;

for i in milestones:
    print('achievements.push_back({ 'f'"Upgrade Achievement {counter}", ' + f'"Purchase {milestones[counter]} Upgrades", AchievementType::UpgradeCount, {milestones[counter]} ' +  '});');
    counter += 1;
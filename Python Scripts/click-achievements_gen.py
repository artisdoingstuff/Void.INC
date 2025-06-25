milestones = [
    1, 10, 100
];

suffixes = [
    (10**3, "K"),
    (10**6, "M"),
    (10**9, "B")
];

def sub100b():
    suffix_counter = 0;
    counter = 0;
    for i in suffixes:
        for j in milestones:
            if milestones[counter] == 250 and suffixes[suffix_counter][1] == "B":
                break;
            print('achievements.push_back({ "", ' + f'"Generate {milestones[counter]}{suffixes[suffix_counter][1]} Bubbles from Clicking", AchievementType::Clicks, {str(suffixes[suffix_counter][0] * milestones[counter])}.0' + ' });');
            counter += 1;
        suffix_counter += 1;
        counter -= counter;

sub100b();
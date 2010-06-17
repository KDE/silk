var acts = activityIds;
print(acts);
for (i = 0; i < acts.length; ++i) {
  print(acts[i]);
  var activity = activityById(acts[i])
  print(activity.name);
  if (activity.name == "rightDesktop") {
    print("We have a desktop!");
    var twitter = activity.addWidget("news");
    //twitter.writeConfig("username", "$USER");
    //twitter.writeConfig("password", "$PASS");
    twitter.showConfigurationInterface();

    sleep(4000);
    twitter.remove();
  }
}



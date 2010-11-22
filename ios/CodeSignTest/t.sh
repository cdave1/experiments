CodeSign /Users/david/Documents/Projects/Research/CodeSignTest/build/Release-iphoneos/CodeSignTest.app
    cd /Users/david/Documents/Projects/Research/CodeSignTest
    setenv CODESIGN_ALLOCATE /Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/codesign_allocate
    setenv PATH "/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin:/Developer/usr/bin:/usr/bin:/bin:/usr/sbin:/sbin"
    /usr/bin/codesign -f -s "iPhone Developer: David Petrie (9B2PDYT3T9)" --resource-rules=/Users/david/Documents/Projects/Research/CodeSignTest/build/Release-iphoneos/CodeSignTest.app/ResourceRules.plist --entitlements /Users/david/Documents/Projects/Research/CodeSignTest/build/CodeSignTest.build/Release-iphoneos/CodeSignTest.build/CodeSignTest.xcent /Users/david/Documents/Projects/Research/CodeSignTest/build/Release-iphoneos/CodeSignTest.app

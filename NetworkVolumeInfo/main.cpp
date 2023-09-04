//
//  main.cpp
//  NetworkVolumeInfo
//
//  Created by Vijay Negi on 04/09/23.
//

#include <iostream>
#include <Cocoa/Cocoa.h>

void CheckAttributesAPI(NSURL* url);
void CheckResourceValueAPI(NSURL* url);

int main(int argc, const char * argv[]) {

    int mode = 3;
    if(argc == 2){
        mode = std::atoi(argv[1]);
    }
    std::cout << "Starting Volume count\n";
    std::uint32_t count = 0;



    NSFileManager*  aFileManager = [NSFileManager defaultManager];

    //Enumerating Hidden Volumes to get path-based URLs.
    NSVolumeEnumerationOptions aEOptions = 0;
    aEOptions = NSVolumeEnumerationSkipHiddenVolumes;

    NSArray *aMountedVolumeUrls = [aFileManager mountedVolumeURLsIncludingResourceValuesForKeys:nil options:aEOptions];

    count = (std::uint32_t)[aMountedVolumeUrls count];

    std::cout << "\nVolume Count detected = "<<count<<std::endl;

    for (NSURL* url in aMountedVolumeUrls)
    {
        NSLog(@"\n -----------------------");
        NSLog(@"absoluteURL = %@", [url absoluteString]);
        CFURLRef cfStr = (__bridge CFURLRef)url;
        Boolean isRechable = CFURLResourceIsReachable(cfStr,NULL);
        NSLog(@"isRechable = %d", isRechable);
        Boolean isHidden = false;
        if(isRechable)
        {
            CFBooleanRef aResultRef;
            if ( CFURLCopyResourcePropertyForKey(cfStr,
                                                 kCFURLIsHiddenKey,
                                                 &aResultRef,
                                                 NULL))
            {
                if(aResultRef != NULL)
                {
                    isHidden = CFBooleanGetValue(aResultRef);
                    CFRelease(aResultRef);
                }
            }
            NSLog(@"isHidden = %d", isHidden);

            if(mode & 1)
            {
                CheckAttributesAPI(url);
            }

            // Now other method
            if(mode & 2)
            {
                CheckResourceValueAPI(url);
            }

        }
    }


    return 0;
}

void CheckAttributesAPI(NSURL* url)
{
    NSFileManager*  aFileManager = [NSFileManager defaultManager];
    NSDictionary  *aFileAttributes = nil;
    NSError       *aError          = nil;
    aFileAttributes                = [aFileManager attributesOfItemAtPath:[url path] error: &aError];
    if (aFileAttributes == nil)
        NSLog(@"Error : attributesOfItemAtPath  = %@", aError);
    else
    {
        NSLog(@"aFileAttributes  = %@", aFileAttributes);
        //                for (NSString *key in aFileAttributes) {
        //                    id value = aFileAttributes[key];
        //                    NSLog(@"%@ : %@", key, value );
        //                }
    }
}

void CheckResourceValueAPI(NSURL* url)
{
    NSError *error;
    NSArray *keys = [NSArray arrayWithObjects:NSURLNameKey, NSURLLocalizedNameKey,
                                              NSURLIsRegularFileKey, NSURLIsDirectoryKey,NSURLIsAliasFileKey,
                     NSURLIsVolumeKey, NSURLIsSymbolicLinkKey, NSURLFileAllocatedSizeKey,
                     NSURLFileProtectionKey, NSURLFileContentIdentifierKey,
                     NSURLFileResourceIdentifierKey, NSURLFileResourceTypeKey, NSURLFileSizeKey,
                     NSURLMayHaveExtendedAttributesKey, NSURLVolumeAvailableCapacityKey,NSURLVolumeTotalCapacityKey,
                     NSURLVolumeIsAutomountedKey, NSURLVolumeIsBrowsableKey, NSURLVolumeIsEjectableKey,
                     NSURLVolumeIsEncryptedKey, NSURLVolumeIsInternalKey, NSURLVolumeIsJournalingKey,
                     NSURLVolumeIsLocalKey, NSURLVolumeIsReadOnlyKey, NSURLVolumeIsRemovableKey,
                     NSURLVolumeIsRootFileSystemKey, NSURLIsMountTriggerKey,
                     NSURLVolumeCreationDateKey, NSURLVolumeIdentifierKey ,NSURLVolumeLocalizedFormatDescriptionKey,
                     NSURLVolumeLocalizedNameKey, NSURLVolumeNameKey, NSURLVolumeResourceCountKey,
                     NSURLVolumeSupportsAccessPermissionsKey, NSURLVolumeSupportsExtendedSecurityKey,
                     NSURLVolumeSupportsHardLinksKey, NSURLVolumeSupportsPersistentIDsKey, NSURLVolumeURLKey,
                     NSURLVolumeUUIDStringKey,
                     NSURLContentAccessDateKey, NSURLContentModificationDateKey, NSURLCreationDateKey,
                     NSURLIsHiddenKey, NSURLIsReadableKey, NSURLIsWritableKey,NSURLPathKey,
                     nil];
    NSDictionary *results = [url resourceValuesForKeys:keys error:&error];
    if(results == nil)
        NSLog(@"Error : resourceValuesForKeys  = %@", error);
    else
    {
        NSLog(@"resource values  = %@", results);
    }
}


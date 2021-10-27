#! /bin/bash

clear
echo
read -p "ACL not enabled. Would you like to enable ACL?(Y/N): " confirm                #confirm to enable ACL from User  

if [[ $confirm == [nN] ]]; then                                          #if yes, enable ACL and remount
        echo "ACL not enabled"
        echo
else
        sudo tune2fs -o +acl /dev/root
        sudo mount -o remount /
        echo "ACL enabled"
        echo
fi

read -p "Enter UserId: " user_id                # input User_id and Group_id
echo
read -p "Enter Group Id: " group_id

while [ $user_id -gt 16777215 ] || [ $user_id -lt 1 ]; do           # check if user_id valid
        echo "User-id Invalid"
        read -p "Enter UserId: " user_id
done

while [ $group_id -gt 16777215 ] || [ $group_id -lt 1 ];do           # check if group_id valid
        echo "Group-id Invalid"
        read -p "Enter GroupId: " group_id
done

user_path="/home/satyu/Desktop/Shubham/Secrets/Users/u"    #user_id path
mkdir $user_path"$user_id"

group_path="/home/satyu/Desktop/Shubham/Secrets/Groups/g"    #group_id path
mkdir $group_path"$group_id"

Users_Folders_name=(
        "/Files"
        "/Passwords"
        "/Crypto_keys"
        "/Calenders"
        "/Diary"
        "/Emails"
        "/CloudCoin"
        "/Contacts"
        )

Groups_Folders_name=(
        "/Files"
        "/Journal"
        "/Messages"
        "/Group_profiles" 
        "/User_profiles" 
        )

for folders_name in ${Users_Folders_name[*]}; do
        mkdir $user_path"$user_id"$folders_name
done

for folders_name in ${Groups_Folders_name[*]}; do
        mkdir $group_path"$group_id"$folders_name
done
echo
sudo useradd "u""$user_id"          #Creating a User with Specific User ID>
echo "Your User Acoount Name: u$user_id"
sudo passwd "u""$user_id"

sudo chown -R "u""$user_id": $user_path"$user_id"     #change owernship of user_id dir and sub dir to user_id>

sudo chown -R "u""$user_id": $group_path"$group_id"   #change owernship of group_id dir and sub dir to user_id>

File_path=$group_path"$group_id""/Files"

sudo touch $File_path/read.txt $File_path/write.txt $File_path/Secret.txt
echo
sudo useradd "g""$group_id"         #Creating a group account with Specific Group ID>
echo "Your 2nd User Account Name: g$group_id"
sudo passwd "g""$group_id"
echo
sudo setfacl -m u:"g""$group_id":r $File_path/read.txt
sudo setfacl -m u:"g""$group_id":w $File_path/write.txt
sudo setfacl -m u:"g""$group_id":--- $File_path/Secret.txt



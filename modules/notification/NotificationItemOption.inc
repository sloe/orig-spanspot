<?php
/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * This ItemAddOption sends a Notification event for the newly created item.
 * @package Notification
 * @subpackage UserInterface
 * @author Zimzat <zimzat@zimzat.com>
 * @version $Revision: 847 $
 */
class NotificationItemOption extends ItemAddOption {

    /**
     * @see ItemAddOption::isAppropriate
     */
    function isAppropriate() {
	return array(null, true);
    }

    /**
     * @see ItemAddOption::handleRequestAfterAdd
     */
    function handleRequestAfterAdd($form, $items) {
	$errors = $warnings = array();

	/* Get the notification system if it's activate */
	list ($ret, $notificationSystem) = GalleryCoreApi::newFactoryInstance(
	    'NotificationSystemInterface_1_0');
	if ($ret) {
	    return array($ret, null, null);
	} else if (!isset($notificationSystem)) {
	    return array(null, $errors, $warnings);
	}

	/* How should we handle what type it is and where to link? */
	$itemCount = count($items);
	if ($itemCount == 1) {
	    /* One item, call it what it is and link to it directly */
	    $itemTypeName = $items[0]->itemTypeName();
	    $itemTypeName = $itemTypeName[1];
	    $itemId = $items[0]->getId();
	    $parentId = $items[0]->getParentId();
	} else {
	    /* Many items, link to the parent */
	    $parentId = $itemId = $items[0]->getParentId();
	    $itemTypes = array();
	    foreach ($items as $item) {
		$itemTypeName = $item->itemTypeName();
		$itemTypeName = $itemTypeName[1];
		if (empty($itemTypes[$itemTypeName])) {
		    $itemTypes[$itemTypeName] = 1;
		} else {
		    $itemTypes[$itemTypeName]++;
		}
	    }

	    /* Call it all of the items */
	    if (count($itemTypes) == 1) {
		$itemTypeNames = array_keys($itemTypes);
	    } else {
		$itemTypeNames = implode(', ', array_keys($itemTypes));
	    }
	}

	/* Get the URL to the item the event was triggered on */
	global $gallery;
	$urlGenerator =& $gallery->getUrlGenerator();
	$parentUrl = $urlGenerator->generateUrl(
	    array(
		'view' => 'core.ShowItem',
		'itemId' => $itemId),
	    array(
		'forceFullUrl' => true,
		'forceSessionId' => false,
		'htmlEntities' => false));

	/* Build the event information to be passed on to the Notification system */
	$eventInformation = array(
	    'itemId' => $parentId,
	    'postFilter' => array(
		'permissionId' => $itemId),
	    'short' => array($itemTypeName),
	    'long' => array($itemTypeName, $parentUrl));
	list ($ret, $status) = $notificationSystem->postEvent(
	    'NotificationEventItemAdd', $eventInformation);
	if ($ret) {
	    return array($ret, null, null);
	}

	return array(null, $errors, $warnings);
    }
}
?>

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
 * Event Notification by Method.
 * Manages subscription and notification of custom events.
 *
 * @package Notification
 * @author Zimzat <zimzat@zimzat.com>
 * @version $Revision: 847 $
 */
class NotificationModule extends GalleryModule {

    function NotificationModule() {
	global $gallery;
	$this->setId('notification');
	$this->setName($gallery->i18n('Notification'));
	$this->setDescription(
		$gallery->i18n('Allow users to be notified of events by subscription.'));
	$this->setGroup('gallery', $gallery->i18n('Gallery'));
	$this->setVersion('0.9.0');
	$this->setRequiredCoreApi(array(7, 10));
	$this->setRequiredModuleApi(array(3, 1));
	$this->setCallbacks('getSiteAdminViews|getUserAdminViews|getItemLinks');
    }

    /**
     * @see GalleryModule::performFactoryRegistrations
     */
    function performFactoryRegistrations() {
	/* Notification System */
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'NotificationSystemInterface_1_0', 'NotificationSystem', 'notificationsystem',
	    'modules/notification/classes/NotificationSystem.class', 'notification', null);
	if ($ret) {
	    return $ret;
	}

	/* Email Method */
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'NotificationMethodInterface_1_0', 'NotificationMethodEmail', 'notificationmethodemail',
	    'modules/notification/classes/NotificationMethodEmail.class', 'notification', null, 4);
	if ($ret) {
	    return $ret;
	}

	/* Item Add Event */
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'NotificationEventInterface_1_0', 
	    'NotificationEventItemAdd', 'notificationeventitemadd', 
	    'modules/notification/classes/NotificationEventItemAdd.class', 'notification', null);
	if ($ret) {
	    return $ret;
	}
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'ItemAddOption', 'NotificationItemOption', 'notificationitemoption',
	    'modules/notification/NotificationItemOption.inc', 'notification', null);
	if ($ret) {
	    return $ret;
	}

	return null;
    }
 
     /**
     * @see GalleryModule::activate
     */
    function activate($postActivationEvent=true) {
	/*
	 * Manually create an instance of the NotificationSystem since performFactoryRegistrations
	 * hasn't been run yet, and running it first will cause the module to be activated even if 
	 * there is an error during activation.
	 */
	GalleryCoreApi::requireOnce('modules/notification/classes/NotificationSystem.class');
	$notificationSystem = new NotificationSystem;

	/* Register all existing method implementations */
	list ($ret, $implementations) = 
		GalleryCoreApi::getAllFactoryImplementationIds('NotificationMethodInterface_1_0');
	if ($ret) {
	    return array($ret, null);
	}
	/* Manually add our methods that are in this module */
	$implementations[] = 'NotificationMethodEmail';
	foreach ($implementations as $methodClass) {
	    list ($ret, $null) = $notificationSystem->registerMethod($methodClass);
	    if ($ret) {
		return array($ret, null);
	    }
	}

	/* Register all existing event implementations */
	list ($ret, $implementations) = 
		GalleryCoreApi::getAllFactoryImplementationIds('NotificationEventInterface_1_0');
	if ($ret) {
	    return array($ret, null);
	}
	/* Manually add our events that are in this module */
	$implementations[] = 'NotificationEventItemAdd';
	foreach ($implementations as $eventClass) {
	    list ($ret, $null) = $notificationSystem->registerEvent($eventClass);
	    if ($ret) {
		return array($ret, null);
	    }
	}

	return parent::activate($postActivationEvent);
    }

    /**
     * @see GalleryModule::getSiteAdminViews
     */
    function getSiteAdminViews() {
	return array(null, array(array(
	    'name' => $this->translate('Notification'),
	    'view' => 'notification.NotificationSiteAdmin')));
    }

    /**
     * @see GalleryModule::getUserAdminViews();
     */
    function getUserAdminViews($user) {
	$views = array();

	list ($ret, $isAnonymous) = GalleryCoreApi::isAnonymousUser($user->getId());
	if ($ret) {
	    return array($ret, null);
	}
	if (!$isAnonymous) {
	    $views[] = array(
		'name' => $this->translate('Watch List'), 
		'view' => 'notification.NotificationUserAdmin', 
	    );
	}

	return array(null, $views);
    }

    /**
     * @see GalleryModule::getItemLinks
     */
    function getItemLinks($items, $wantsDetailedLinks, $permissions, $userId) {
	list ($ret, $isAnonymous) = GalleryCoreApi::isAnonymousUser($userId);
	if ($ret) {
	    return array($ret, null);
	} else if ($isAnonymous) {
	    return array(null, array());
	}

	$links = array();
	$params['view'] = 'notification.NotificationItem';
	foreach ($items as $item) {
	    $params['itemId'] = $item->getId();
	    $links[$item->getId()][] = array(
		'text' => $this->translate('Watch'),
		'params' => $params, 
	    );
	}

	return array(null, $links);
    }
}
?>

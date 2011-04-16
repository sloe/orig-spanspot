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

GalleryCoreApi::requireOnce('modules/notification/classes/NotificationHelper.class');

/**
 * This controller saves changes to settings on a per-user basis.
 * @package Notification
 * @subpackage UserInterface
 * @author Zimzat <zimzat@zimzat.com>
 * @version $Revision: 847 $
 */
class NotificationUserAdminController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	$redirect = $status = $error = array();

	/* Check that the user is logged in */
	list ($ret, $userId) = NotificationHelper::getUserIdCheck();
	if ($ret) {
	    return array($ret, null);
	}

	$redirect = array(
	    'view' => 'core.UserAdmin', 
	    'subView' => 'notification.NotificationUserAdmin');

	if (isset($form['action']['save'])) {
	    /* Load the Notification System */
	    list ($ret, $notificationSystem) = GalleryCoreApi::newFactoryInstance(
		'NotificationSystemInterface_1_0');
	    if ($ret) {
		return array($ret, null);
	    } else if (!isset($notificationSystem)) {
		return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
	    }

	    /* Get Enabled Methods */
	    list ($ret, $methods) = NotificationHelper::getMethods(NOTIFICATION_RETURN_BASIC);
	    if ($ret) {
		return array($ret, null);
	    }

	    /* Get Enabled Events */
	    list ($ret, $events) = NotificationHelper::getEvents(NOTIFICATION_RETURN_BASIC);
	    if ($ret) {
		return array($ret, null);
	    }

	    /* Get Subscriptions */
	    list ($ret, $subscriptions) = $notificationSystem->getSubscriptions(
		$events, array('userId' => $userId));
	    if ($ret) {
		return array($ret, null);
	    }

	    /* Check events against form input for changes */
	    $methodClassChanges = array();
	    foreach ($events as $eventClass) {
		/* Easy access to the form elements */
		unset($eventForm); /* Avoid reference overwriting */
		$eventForm =& $form['events'][$eventClass];

		/* Check for unsubscribe requests */
		if (!empty($eventForm['unsubscribe']) && is_array($eventForm['unsubscribe'])) {
		    /* Check form input against existing subscriptions */
		    $unsubscribeFilter = array();
		    foreach (array_keys($eventForm['unsubscribe']) as $filter) {
			/* Check for valid item filter */
			if (!array_key_exists($filter, $subscriptions[$eventClass])) {
			    $errors[] = 'form[error][subscription][unsubscribe][filter][invalid]';
			    continue;
			}

			$unsubscribeFilter[] = $filter;

			/* Don't attempt to change the methodClass after unsubscribing */
			unset($eventForm['methodClass'][$filter]);
		    }

		    /* Unsubscribe all of the requested existing subscriptions */
		    if (!empty($unsubscribeFilter)) {
			list ($ret, $eventStatus) = $notificationSystem->unsubscribeEvent(
			    $eventClass, $userId, $unsubscribeFilter);
			if ($ret) {
			    return array($ret, null);
			}
		    }
		}

		/* Check for methodClass changes */
		if (!empty($eventForm['methodClass']) && is_array($eventForm['methodClass'])) {
		    /* Check form input against existing subscriptions */
		    foreach ($eventForm['methodClass'] as $filter => $methodClass) {
			/* Check for valid methodClass */
			if (!in_array($methodClass, $methods)) {
			    $errors[] = 'form[error][subscription][methodClass][invalid]';
			    continue;
			}

			/* Check for valid filter */
			if (!array_key_exists($filter, $subscriptions[$eventClass])) {
			    $errors[] = 'form[error][subscription][filter][invalid]';
			    continue;
			}

			/* Check for changed methodClass */
			if ($subscriptions[$eventClass][$filter]['methodClass'] != $methodClass) {
			    $methodClassChanges[$methodClass][] = $filter;
			}
		    }
		}

		/* Change methodClasses in the map */
		$match = array('eventClass' => $eventClass, 'userId' => $userId);
		foreach ($methodClassChanges as $methodClass => $filters) {
		    $match['filter'] = $filters;
		    $change = array('methodClass' => $methodClass);
		    $ret = GalleryCoreApi::updateMapEntry(
			'NotificationSubscriptions', $match, $change);
		    if ($ret) {
			return array($ret, null);
		    }
		}
	    }

	    /* Alert of the changes we have done. */
	    $status['saved'] = 1;
	}

	$results['redirect'] = $redirect;
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }
}

/**
 * This view shows all event / method settings on a per-user basis.
 */
class NotificationUserAdminView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	/* Check that the user is logged in */
	list ($ret, $userId) = NotificationHelper::getUserIdCheck();
	if ($ret) {
	    return array($ret, null);
	}

	$methods = $methodOptions = $events = $eventInstances = $itemIds = 
	    $NotificationSubscriptions = $NotificationItems = array();

	/* Get all available methods */
	list ($ret, $methods) = NotificationHelper::getMethods();
	if ($ret) {
	    return array($ret, null);
	}
	foreach ($methods as $methodClass => $method) {
	    $methodOptions[$methodClass] = $method['methodName'];
	}

	/* Get all available events */
	list ($ret, $events) = NotificationHelper::getEvents();
	if ($ret) {
	    return array($ret, null);
	}

	/* Subscriptions */
	list ($ret, $notificationSystem) = GalleryCoreApi::newFactoryInstance(
	    'NotificationSystemInterface_1_0');
	if ($ret) {
	    return array($ret, null);
	} else if (!isset($notificationSystem)) {
	    return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT), null);
	}
	list ($ret, $subscriptions) = $notificationSystem->getSubscriptions(
	    array_keys($events), array('userId' => $userId));
	if ($ret) {
	    return array($ret, null);
	}

	/* Go through each subscription to get the itemIds */
	foreach ($subscriptions as $eventClass => $eventSubscriptions) {
	    foreach ($eventSubscriptions as $itemSubscription) {
		if ($itemSubscription['filter']{0} != 'i') {
		    continue;
		}

		list ($null, $itemId) = explode('=', $itemSubscription['filter']);
		$itemIds[$itemId] = true;
	    }
	    $NotificationSubscriptions[$eventClass] = $eventSubscriptions;
	}

	/* Load each item and get its title for the page */
	if (!empty($itemIds)) {
	    global $gallery;
	    $urlGenerator =& $gallery->getUrlGenerator();
    
	    list ($ret, $items) = GalleryCoreApi::loadEntitiesById(array_keys($itemIds));
	    if ($ret) {
		return array($ret, null);
	    }
	    foreach ($items as $item) {
		$itemUrl = $urlGenerator->generateUrl(
		    array('view' => 'core.ShowItem', 'itemId' => $item->getId()));
    
		$NotificationItems['i=' . $item->getId()] = array(
		    'title' => $item->getTitle(), 
		    'url' => $itemUrl);
	    }
	}

	$template->setVariable('NotificationMethods', $methods);
	$template->setVariable('NotificationMethodOptions', $methodOptions);
	$template->setVariable('NotificationEvents', $events);
	$template->setVariable('NotificationSubscriptions', $NotificationSubscriptions);
	$template->setVariable('NotificationItems', $NotificationItems);
	$template->setVariable('controller', 'notification.NotificationUserAdmin');

	return array(null, 
		     array('body' => 'modules/notification/templates/NotificationUserAdmin.tpl'));
    }
}
?>

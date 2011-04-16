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
 * This controller handles event / method settings on a site-wide basis.
 * @package Notification
 * @subpackage UserInterface
 * @author Zimzat <zimzat@zimzat.com>
 * @version $Revision: 847 $
 */
class NotificationSiteAdminController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	$redirect = $status = $error = array();

	/* Check if logged in and is admin */
	list ($ret, $userId) = NotificationHelper::getUserIdCheck(true);
	if ($ret) {
	    return array($ret, null);
	}

	$redirect = array(
	    'view' => 'core.SiteAdmin', 
	    'subView' => 'notification.NotificationSiteAdmin');

	if (!empty($form['action']['save'])) {
	    $change = array('enable' => array(), 'disable' => array());
	    $change = array('methods' => $change, 'events' => $change);

	    /* Methods */
	    if (empty($form['methods']) || is_array($form['methods'])) {
		list ($ret, $methods) = NotificationHelper::getMethods(
		    NOTIFICATION_RETURN_BASIC, false);
		if ($ret) {
		    return array($ret, null);
		}

		foreach ($methods as $methodClass => $enabled) {
		    if (empty($form['methods'][$methodClass])) {
			if ($enabled) {
			    $change['methods']['disable'][] = $methodClass;
			}
		    } else {
			if (!$enabled) {
			    $change['methods']['enable'][] = $methodClass;
			}
		    }
		}
	    }

	    /* Events */
	    if (empty($form['events']) || is_array($form['events'])) {
		list ($ret, $events) = NotificationHelper::getEvents(
		    NOTIFICATION_RETURN_BASIC, false);
		if ($ret) {
		    return array($ret, null);
		}
		foreach ($events as $eventClass => $enabled) {
		    if (empty($form['events'][$eventClass])) {
			if ($enabled) {
			    $change['events']['disable'][] = $eventClass;
			}
		    } else {
			if (!$enabled) {
			    $change['events']['enable'][] = $eventClass;
			}
		    }
		}
	    }

	    /* Change all methods/events at the same time */
	    foreach ($change as $type => $actions) {
		if ($type == 'methods') {
		    $map = 'NotificationMethods';
		    $className = 'methodClass';
		} else {
		    $map = 'NotificationEvents';
		    $className = 'eventClass';
		}

		foreach ($actions as $action => $classes) {
		    if (empty($classes)) {
			continue;
		    }

		    if ($action == 'disable') {
			$fieldChange = array('enabled' => false);
		    } else {
			$fieldChange = array('enabled' => true);
		    }

		    $ret = GalleryCoreApi::updateMapEntry(
			$map, array($className => $classes), $fieldChange);
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
 * This view shows all the event / method settings on a site-wide basis.
 */
class NotificationSiteAdminView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	/* Check if logged in and is admin */
	list ($ret, $userId) = NotificationHelper::getUserIdCheck(true);
	if ($ret) {
	    return array($ret, null);
	}

	/* Get all methods */
	list ($ret, $methods) = NotificationHelper::getMethods(NOTIFICATION_RETURN_FULL, false);
	if ($ret) {
	    return array($ret, null);
	}

	/* Get all events */
	list ($ret, $events) = NotificationHelper::getEvents(NOTIFICATION_RETURN_FULL, false);
	if ($ret) {
	    return array($ret, null);
	}

	$template->setVariable('NotificationMethods', $methods);
	$template->setVariable('NotificationEvents', $events);
	$template->setVariable('controller', 'notification.NotificationSiteAdmin');

	return array(null, 
		     array('body' => 'modules/notification/templates/NotificationSiteAdmin.tpl'));
    }
}
?>

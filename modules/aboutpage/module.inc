<?php
/*
 * $RCSfile: module.inc,v $
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2005 Bharat Mediratta
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
 
/**
 * 
 * @package AboutPage
 * @author Darren Guiden <g2@pengs.co.uk>
 */
 
/**
 * Display a page that user can put own content on
 *
 * @package AboutPage
 */
class AboutPageModule extends GalleryModule {

    function AboutPageModule() {
	global $gallery;		
	$this->setId('aboutpage');
	$this->setName('About Page');
	$this->setDescription($gallery->i18n('Display a page of user content'));
	$this->setVersion('0.0.2');
	$this->setGroup('display', $gallery->i18n('Display'));
	$this->setCallbacks('getSystemLinks|getSiteAdminViews');
	$this->setRequiredCoreApi(array(7, 2));
	$this->setRequiredModuleApi(array(3, 0));
    }

    /**
     * @see GalleryModule::needsConfiguration
     */
    function needsConfiguration() {
        list ($ret, $value) = $this->getParameter('message');
        if ($ret) {
            return array($ret->wrap(__FILE__, __LINE__), null);
        }
        return array(null, empty($value));
    }

    /**
     * @see GalleryModule::getSiteAdminViews
     */
    function getSiteAdminViews() {
	return array(null, array(array('name' => $this->translate('About'), 
							   'view' => 'aboutpage.Admin')));
    }

    /**
     * @see GalleryModule::getSystemLinks
     */
    function getSystemLinks() {
	$links = array();

      $links['AboutPage'] = array(
		'text' => $this->translate('About'),
		'params' => array('view' => 'aboutpage.About'));

	return array(null, $links);
    }

    /**
     * @see GalleryModule::getConfigurationView
     */
    function getConfigurationView() {
	return 'aboutpage.Admin';
    }


  /**
     * @see GalleryModule::getRewriteRules
     */
    function getRewriteRules() {
	return array(
		array('comment' => $this->translate('About Page'),
		      'match' => array('view' => 'aboutpage.About'),
		      'pattern' => 'about',
		      'help' => $this->translate('About Page') )
		);
    }


}
?>
